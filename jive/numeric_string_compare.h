/**
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright 2018 Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  */

#pragma once

#include <string_view>
#include <string>
#include <map>
#include <vector>

namespace jive
{


namespace detail
{


class Chunk
{
public:
    Chunk(std::string_view valueAsString, bool isNumeric);

    // Comparison operators compare as int only if both values are int.
    // Otherwise, compare as strings
    bool operator<(const Chunk &other) const;
    bool operator>(const Chunk &other) const;

private:
    bool isNumeric_;
    std::string_view valueAsString_;
    int valueAsInt_;
};


class NumericString
{
public:
    NumericString() = default;

    NumericString(std::string_view value);

    bool operator<(const NumericString &other) const;

    bool operator==(const NumericString &other) const;

    explicit operator std::string_view () const { return this->value_; }

private:
    std::string_view value_;
    std::vector<Chunk> chunks_;
};


std::ostream & operator<<(std::ostream &, const NumericString &);


} // end namespace detail


class NumericStringCompare
{
public:
    bool operator()(std::string_view first, std::string_view second) const;
};


using SortedStringMap =
    std::map<std::string, std::string, NumericStringCompare>;


} // namespace jive
