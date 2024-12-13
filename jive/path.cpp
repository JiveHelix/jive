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
#include "jive/error.h"

#include <errno.h>
#include <cstring>
#include <cstdint>
#include <deque>
#include <utility>

#ifdef _WIN32
// Request definition of S_IFMT, S_IFREG, and S_IFDIR
#define _CRT_INTERNAL_NONSTDC_NAMES 1

#include <algorithm>

#endif

#include <sys/types.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <direct.h>
#define stat _stat

#if !defined(S_ISREG) && defined(S_IFMT) && defined(S_IFREG)
#define S_ISREG(m) (((m) & S_IFMT) == S_IFREG)
#endif

#if !defined(S_ISDIR) && defined(S_IFMT) && defined(S_IFDIR)
#define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)
#endif

#else
#include <unistd.h>
#endif


namespace jive
{


namespace path
{


std::string & FilterSeparators(std::string &path)
{

#ifdef _WIN32
    std::replace(path.begin(), path.end(), backwardsSeparator, pathSeparator);
#endif

    // Remove repeated separators.
    auto last = std::unique(
        std::begin(path),
        std::end(path),
        [] (char left, char right) -> bool
        {
            return left == pathSeparator && right == pathSeparator;
        });

    path.erase(last, std::end(path));

    return path;
}


// InputIterator must dereference to a std::string
template<typename InputIterator>
static std::string Join(InputIterator first, InputIterator last)
{
    std::string result = strings::Join(first, last, pathSeparator);

    // Replace all backslashes with forward slashes.
    FilterSeparators(result);

    return result;
}


std::string Join(std::initializer_list<std::string> il)
{
    return Join(il.begin(), il.end());
}


std::string Join(const std::string& path1, const std::string& path2)
{
    return Join({path1, path2});
}


std::string Base(const std::string& fileName)
{
    std::string filtered = fileName;
    FilterSeparators(filtered);

    std::string::size_type pos = filtered.find_last_of(pathSeparator);

    if (pos == std::string::npos)
    {
        return filtered;
    }

    return filtered.substr(pos + 1);
}


std::string Directory(const std::string& fileName)
{
    std::string filtered = fileName;
    FilterSeparators(filtered);

    std::string::size_type pos = fileName.find_last_of(pathSeparator);

    if (pos == std::string::npos)
    {
        return "";
    }

    return fileName.substr(0, pos);
}


std::pair<std::string, std::string> Split(const std::string& fileName)
{
    std::string filtered(fileName);
    FilterSeparators(filtered);
    
    std::string::size_type pos = filtered.find_last_of(pathSeparator);

    if (pos == std::string::npos)
    {
        return std::make_pair("", filtered);
    }

    return std::make_pair(
        filtered.substr(0, pos),
        filtered.substr(pos + 1));
}


std::pair<std::string, std::string> SplitExtension(const std::string& fileName)
{
    std::string::size_type pos = fileName.find_last_of('.');

    if (pos == std::string::npos)
    {
        return std::make_pair(fileName, "");
    }

    return std::make_pair(fileName.substr(0, pos), fileName.substr(pos));
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


bool Exists(const std::string &name)
{
    static struct stat st;
    return (stat(name.c_str(), &st) == 0);
}


#ifndef _WIN32
bool IsFifo(const std::string &name)
{
    struct stat st;
    if (stat(name.c_str(), &st) == 0)
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
#endif


bool IsFile(const std::string &name)
{
    struct stat st;
    if (stat(name.c_str(), &st) == 0)
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


bool IsDirectory(const std::string &name)
{
    struct stat st;

    if (stat(name.c_str(), &st) == 0)
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


#ifndef _WIN32
void MakeFifo(const std::string &fifoName)
{
    static constexpr mode_t mode{0644};

    if (mkfifo(fifoName.c_str(), mode) != 0)
    {
        throw PathError(
            SystemError(errno),
            "MakeFifo(" + fifoName + ") failed");
    }
}
#endif


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

    if (result != -1)
    {
        // Success.
        return;
    }

    if (errno == EEXIST)
    {
        // An existing item is only an error if it is not a directory.
        if (!IsDirectory(pathName))
        {
            throw PathError(
                SystemError(errno),
                "Failed to create directory "
                    + pathName
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

        throw PathError(SystemError(errno), errorMessage);
    }
}


void MakeDirectories(const std::string &pathname)
{
    if (pathname.empty())
    {
        return;
    }

    auto filtered = pathname;
    FilterSeparators(filtered);

    auto subDirectoryVector = strings::Split(filtered, pathSeparator);

    if (subDirectoryVector.empty())
    {
        return;
    }

    std::deque<std::string> subDirectories(
        subDirectoryVector.begin(),
        subDirectoryVector.end());

    std::deque<std::string> pathsToCreate;

    if (subDirectories.front().empty())
    {
        // path is absolute, beginning with '/'
        // strings::Split left an empty string in the front representing that
        // nothing came before the initial '/'
        subDirectories.pop_front();

        // The first directory needs to be begin with a '/'
        pathsToCreate.emplace_back(
            std::string(1, pathSeparator) + subDirectories.front());
    }
    else
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

    for (const auto &subdir: pathsToCreate)
    {
        MakeDirectory(subdir);
    }
}


time_t GetCreationTime(const std::string &fileName)
{
    struct stat result;
    if (stat(fileName.c_str(), &result) != 0)
    {
        throw PathError(
            std::make_error_code(std::errc::no_such_file_or_directory),
            "Unable to access " + fileName);
    }

    return result.st_ctime;
}


} // end namespace path


} // end namespace jive
