/**
  * @file list_directory.h
  *
  * @brief List all files in a directory.
  *
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright 2011-2018 Jive Helix
  *
  * Licensed under the MIT license. See LICENSE file.
  */

#pragma once

/// Remove in favor of std::filesystem
#ifndef _WIN32
#ifndef __aarch64__

#include <sys/types.h>
#include <dirent.h>

#if !(defined(DT_DIR) && defined(DT_REG))
    #error missing macros DT_DIR and DT_REG.
#endif

#include <string>
#include <vector>
#include <system_error>
#include "jive/create_exception.h"

CREATE_SYSTEM_ERROR(ListDirectoryError, std::system_error);
CREATE_SYSTEM_ERROR(BadDirectory, ListDirectoryError);

namespace jive
{

std::vector<std::string> ListDirectory(const std::string& path);

} // end namespace jive

#endif // __aarch64__
#endif // _WIN32
