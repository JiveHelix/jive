/**
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright 2011-2018 Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  */

#include "jive/numeric_string_compare.h"
#include <vector>
#include <algorithm>
#include <cctype>
#include "jive/strings.h"
#include "jive/to_integer.h"


namespace jive
{

Chunk::Chunk(const std::string &valueAsString, bool isNumeric)
    :
    isNumeric_(isNumeric),
    valueAsString_(valueAsString)
{
     if (isNumeric)
     {
         this->valueAsInt_ = jive::ToInteger<int>(valueAsString);
     }
}

// Comparison operators compare as int only if both values are int.
// Otherwise, compare as strings
bool Chunk::operator<(const Chunk &other) const
{
    if (this->isNumeric_ && other.isNumeric_)
    {
        return this->valueAsInt_ < other.valueAsInt_;
    }
    else
    {
        return this->valueAsString_ < other.valueAsString_;
    }
}

bool Chunk::operator>(const Chunk &other) const
{
    if (this->isNumeric_ && other.isNumeric_)
    {
        return this->valueAsInt_ > other.valueAsInt_;
    }
    else
    {
        return this->valueAsString_ > other.valueAsString_;
    }
}


NumericString::NumericString(const std::string &value)
    :
    value_(value)
{
    if (!value.empty())
    {
        auto chunkBegin = value.begin();
        auto it = chunkBegin;
        bool isDigit = std::isdigit(*it);

        while (it != value.end())
        {
            bool thisIsDigit = std::isdigit(*it);
            if (thisIsDigit != isDigit)
            {
                // This character has a different type than the last.
                // Store the last chunk, and begin a new one.
                this->chunks_.emplace_back(
                    std::string(chunkBegin, it),
                    isDigit);
                
                isDigit = thisIsDigit;
                chunkBegin = it;
            }

            ++it;
        }

        // Add the last chunk
        this->chunks_.emplace_back(
            std::string(chunkBegin, it),
            isDigit);
    }
}

bool NumericString::operator<(const NumericString &other) const
{
    size_t chunkCount =
        std::min(this->chunks_.size(), other.chunks_.size());

    auto thisChunk = this->chunks_.begin();
    auto otherChunk = other.chunks_.begin();

    while (chunkCount--)
    {
        if (*thisChunk < *otherChunk)
        {
            return true;
        }
        else if (*thisChunk > *otherChunk)
        {
            return false;
        }

        thisChunk++;
        otherChunk++;
    }
    
    // Each side compared equal until now
    // Let the shorter side win.
    return (this->chunks_.size() < other.chunks_.size());
}

bool NumericString::operator==(const NumericString &other) const
{
    return (this->value_ == other.value_);
}


bool NumericStringCompare::operator()(
    const std::string& first,
    const std::string& second) const
{
    return (NumericString(first) < NumericString(second));
}

std::ostream & operator<<(
    std::ostream &outputStream,
    const NumericString &numericString)
{
    return outputStream << static_cast<std::string>(numericString);
}

} // namespace jive


