/**
  * @file path.h
  *
  * @brief Tools for file system path manipulation.
  *
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright 2011-2018 Jive Helix
  *
  * License under the MIT license. See LICENSE file.
  */

#pragma once

#include <string>
#include <vector>
#include <utility>
#include <functional>
#include "jive/strings.h"
#include <initializer_list>

#include <sys/stat.h>
#include <errno.h>
#include <map>
#include "jive/create_exception.h"
#include <list>

namespace jive
{

namespace path
{

// The forward slash is a valid path separator for darwin, *nix, and
// Windows NT.
inline constexpr char pathSeparator = '/';

CREATE_EXCEPTION(PathError, std::runtime_error);

std::string Join(std::initializer_list<std::string> il);

std::string Join(const std::string& path1, const std::string& path2);

std::string Base(const std::string& fileName);

std::string Directory(const std::string& fileName);

std::pair<std::string, std::string> Split(const std::string& fileName);

bool Exists(const std::string &name);

bool IsFifo(const std::string &name);

bool IsFile(const std::string &name);

bool IsDirectory(const std::string &name);

void MakeFifo(const std::string &name);

/*
 * @return std::pair, first = stem, second = .extension
 */
std::pair<std::string, std::string>
SplitExtension(const std::string& name);

/*
 *  Appends an index to systemName until it finds a systemName that ! Exists
 */
std::string MakeUniqueSystemName(const std::string &systemName);

void MakeDirectory(const std::string &pathName);

// This function should have the same result as calling `mkdir -p pathname`
void MakeDirectories(const std::string &pathName);

time_t GetCreationTime(const std::string &fileName);

} // namespace path

} // end namespace jive
