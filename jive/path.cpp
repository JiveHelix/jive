/**
  * @file path.cpp
  * 
  * @brief Tools for file system path manipulation.
  * 
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright 2011-2018 Jive Helix
  *
  * License under the MIT license. See LICENSE file.
  */

#include "jive/path.h"

#include <errno.h>
#include <cstring>
#include <deque>
#include <utility>

// For GetLastCreationTime
#include <sys/types.h>
#include <sys/stat.h>
#ifdef _WIN32
#include <direct.h>
#define stat _stat
#else
#include <unistd.h>
#endif

namespace jive
{

namespace path
{

// InputIterator must dereference to a std::string
template<typename InputIterator>
static std::string Join(InputIterator first, InputIterator last)
{
    std::string result = strings::Join(first, last, pathSeparator);
    auto GetAdjacentPathSeparator = 
        [](const char &first, const char &second)->bool
        {
            return first == second && first == pathSeparator;
        };

    auto it = std::adjacent_find(
        result.begin(),
        result.end(),
        GetAdjacentPathSeparator);

    if (it == result.end())
    {
        // There are no adjacent PATH_SEPARATORs
        return result;
    }

    // @todo test the execution speed of list vs vector for removing duplicate
    // path separators.
    // We know that there is at least one extra pathSeparator
    std::list<char> resultAsList(result.begin(), result.end());
    auto listIt = resultAsList.begin();

    while (resultAsList.end() != (listIt = std::adjacent_find(
        listIt,
        resultAsList.end(),
        GetAdjacentPathSeparator)))
    {
        listIt = resultAsList.erase(listIt);
    }

    return std::string(resultAsList.begin(), resultAsList.end());
}

std::string Join(std::initializer_list<std::string> il)
{
    return Join(il.begin(), il.end());
}

std::string Join(const std::string& path1, const std::string& path2)
{
    return Join({path1, path2});
}

std::string Base(const std::string& filename)
{
    std::string::size_type pos = filename.find_last_of(pathSeparator);
    if (pos == std::string::npos)
    {
        return filename;
    } else
    {
        return filename.substr(pos + 1);
    }
}

std::string Directory(const std::string& filename)
{
    std::string::size_type pos = filename.find_last_of(pathSeparator);
    if (pos == std::string::npos)
    {
        return "";
    } else
    {
        return filename.substr(0, pos);
    }
}

std::pair<std::string, std::string> Split(const std::string& filename)
{
    std::string::size_type pos = filename.find_last_of(pathSeparator);
    if (pos == std::string::npos)
    {
        return std::make_pair("", filename);
    } else
    {
        return std::make_pair(
            filename.substr(0, pos),
            filename.substr(pos + 1));
    }
}

std::pair<std::string, std::string> SplitExtension(const std::string& filename)
{
    std::string::size_type pos = filename.find_last_of('.');
    if (pos == std::string::npos)
    {
        return std::make_pair(filename, "");
    } else
    {
        return std::make_pair(filename.substr(0, pos), filename.substr(pos));
    }
}

std::string MakeUniqueSystemName(const std::string &systemName)
{
    auto dirBase = Split(systemName);
    auto splitExt = SplitExtension(dirBase.second);
    uint32_t suffix = 0;
    std::string uniqueName(systemName);
    while (path::Exists(uniqueName)) 
    {
        uniqueName = 
            splitExt.first + "-" + std::to_string(++suffix) + splitExt.second;

        if (dirBase.first.length())
        {
            uniqueName = path::Join(
                dirBase.first,
                uniqueName);
        }
    }

    return uniqueName;
}


bool Exists(const std::string &someName)
{
    static struct stat st;
    return (stat(someName.c_str(), &st) == 0);
}

bool IsFifo(const std::string &fileName)
{
    struct stat st;
    if (stat(fileName.c_str(), &st) == 0)
    {
        // Something with this name exists
        if (S_ISFIFO(st.st_mode))
        {
            // It is a fifo.
            return true;
        }
    }

    return false;
}

bool IsFile(const std::string &fileName)
{
    struct stat st;
    if (stat(fileName.c_str(), &st) == 0)
    {
        // Something with this name exists
        if (S_ISREG(st.st_mode))
        {
            // It is a regular file.
            return true;
        }
    }

    return false;
}

bool IsDirectory(const std::string &fileName)
{
    struct stat st;
    if (stat(fileName.c_str(), &st) == 0)
    {
        // Something with this name exists
        if (S_ISDIR(st.st_mode))
        {
            // It is a directory.
            return true;
        }
    }

    return false;
}

void MakeFifo(const std::string &fifoName)
{
    if (mkfifo(fifoName.c_str(), 0644) < 0)
    {
        throw PathError(
            "MakeFifo(" + fifoName + ") failed: " + std::strerror(errno));
    }
}

std::map<int, std::string> mkdirErrorsByErrno({
    { EACCES, "Search permission is denied on a component of the path "
        "prefix, or write permission is denied on the parent directory "
        "of the directory to be created." },
    { EEXIST, "The named file exists." },
    { ELOOP, "A loop exists in symbolic links encountered during "
        "resolution of the path argument." },
    { EMLINK, "The link count of the parent directory would exceed "
        "{LINK_MAX}." },
    { ENAMETOOLONG, "The length of the path argument exceeds {PATH_MAX} "
        "or a pathname component is longer than {NAME_MAX}." },
    { ENOENT, "A component of the path prefix specified by path does not "
        "name an existing directory or path is an empty string." },
    { ENOSPC, "The file system does not contain enough space to hold the "
        "contents of the new directory or to extend the parent directory "
        "of the new directory." },
    { ENOTDIR, "A component of the path prefix is not a directory." },
    { EROFS, "The parent directory resides on a read-only file system." }
});

void MakeDirectory(const std::string &pathName)
{
#ifdef _WIN32
    auto result = _mkdir(pathName.c_str());
#else
    auto result = mkdir(
        pathName.c_str(),
        S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
#endif

    if (result == -1)
    {
        if (errno == EEXIST)
        {
            if (!IsDirectory(pathName))
            {
                throw PathError(
                    "Failed to create directory " + pathName 
                    + ". File already exists.");
            }
        }
        else
        {
            std::string errorMessage(
                "Failed to create directory: " + pathName + ", ");
            try
            {
                errorMessage += mkdirErrorsByErrno.at(errno);
            } catch (std::out_of_range &)
            {
                errorMessage += "Unknown errno " + std::to_string(errno);
            }

            throw PathError(errorMessage);
        }
    }
}

void MakeDirectories(const std::string &pathname)
{
    if (!pathname.size())
    {
        return;
    }

    auto subDirectoryVector = strings::Split(pathname, pathSeparator);

    if (subDirectoryVector.size() == 0)
    {
        return;
    }
    
    std::deque<std::string> subDirectories(
        subDirectoryVector.begin(),
        subDirectoryVector.end());

    std::deque<std::string> pathsToCreate;
    
    if (subDirectories.front() == "")
    {
        // path is absolute, beginning with '/'
        // strings::Split left an empty string in the front representing that
        // nothing came before the initial '/'
        subDirectories.pop_front();
        
        // The first directory needs to be begin with a '/'
        pathsToCreate.emplace_back(
            std::string(1, pathSeparator) + subDirectories.front());
    } else
    {
        // relative path
        pathsToCreate.emplace_back(subDirectories.front());
    }

    subDirectories.pop_front();
     
    while (!subDirectories.empty())
    {
        pathsToCreate.emplace_back(
            path::Join(pathsToCreate.back(), subDirectories.front()));

        subDirectories.pop_front();
    }

    for (auto subdir: pathsToCreate)
    {
        MakeDirectory(subdir);
    }
}


time_t GetCreationTime(const std::string &fileName)
{
    struct stat result;
    if (stat(fileName.c_str(), &result) != 0)
    {
        throw PathError("Unable to access " + fileName);
    }

    return result.st_ctime;
}


} // end namespace path

} // end namespace jive
