/**
  * @file list_directory.cpp
  *
  * @brief List all files in a directory.
  *
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright 2011-2018 Jive Helix
  *
  * Licensed under the MIT license. See LICENSE file.
  */

#include "jive/list_directory.h"
#include "jive/errno_exception.h"
#include <memory>

namespace jive
{


class ManagedDir
{
public:
    ManagedDir(const std::string &path)
        :
        dir_(opendir(path.c_str()))
    {
        if (!this->dir_)
        {
            throw jive::ErrnoException<BadDirectory>(
                "ListDirectory failed: " + path,
                errno);
        }
    }

    ~ManagedDir()
    {
        closedir(this->dir_);
    }

    /// Allows ManagedDir to be used directly where a DIR * is epxected.
    operator DIR * () { return this->dir_; }

private:
    DIR *dir_;
};


std::vector<std::string> ListDirectory(const std::string &path)
{
    std::vector<std::string> results;
    ManagedDir managedDir(path);

    dirent *entry = nullptr;

    while (true)
    {
        entry = readdir(managedDir);
        if (entry)
        {
            std::string_view name(entry->d_name);
            auto size = name.size();

            if (size == 1)
            {
                if (name[0] == '.')
                {
                    continue;
                }
            }
            else if (size == 2)
            {
                if ((name[0] == '.') && (name[1] == '.'))
                {
                    continue;
                }
            }

            results.emplace_back(name);
        }
        else
        {
            break;
        }
    }

    return results;
}


} // end namespace jive
