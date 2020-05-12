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

/// TODO: Create a windows compatible implementation.
#ifdef _WIN32
#error Needs windows implementation.
#endif

#include <sys/types.h>
#include <dirent.h>

#if !(defined(DT_DIR) && defined(DT_REG))
    #error missing macros DT_DIR and DT_REG.
#endif

#include <string>
#include <vector>
#include "jive/create_exception.h"

CREATE_EXCEPTION(ListDirectoryError, std::runtime_error);
CREATE_EXCEPTION(BadDirectory, ListDirectoryError);

namespace jive
{

std::vector<std::string> ListDirectory(const std::string& path);

} // end namespace jive
