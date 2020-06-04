/**
  * @file format_paragraph.h
  *
  * @brief Converts a string into a vector of strings that fit within a line.
  *
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright 2011-2018 Jive Helix
  *
  * Licensed under the MIT license. See LICENSE file.
  */

#pragma once

#include <vector>
#include <string>

namespace jive
{

std::vector<std::string> FormatParagraph(
    const std::string &paragraph,
    size_t indentSpacesCount,
    size_t maxLineSpacesCount);

} // end namespace jive
