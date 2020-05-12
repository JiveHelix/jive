/**
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright 2011-2018 Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  */

#pragma once

#include <string>
#include <map>
#include <vector>

namespace jive
{

class Chunk
{
public:
    Chunk(const std::string &valueAsString, bool isNumeric);

    // Comparison operators compare as int only if both values are int.
    // Otherwise, compare as strings
    bool operator<(const Chunk &other) const;
    bool operator>(const Chunk &other) const;

private:
    bool isNumeric_;
    std::string valueAsString_;
    int valueAsInt_;
};


class NumericString
{
public:
    NumericString(): value_(), chunks_() {}

    NumericString(const std::string &value);

    bool operator<(const NumericString &other) const;
    
    bool operator==(const NumericString &other) const;

    explicit operator const std::string & () const { return this->value_; }

    const char * c_str() const { return this->value_.c_str(); }
    
private:
    std::string value_;
    std::vector<Chunk> chunks_;
};


class NumericStringCompare
{
public:
    bool operator()(const std::string &first, const std::string &second) const;
};

typedef std::map<
    std::string,
    std::string,
    NumericStringCompare> SortedStringMapT;

std::ostream & operator<<(std::ostream &, const NumericString &);

} // namespace jive
