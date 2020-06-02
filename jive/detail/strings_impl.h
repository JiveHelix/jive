/**
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright 2011-2020 Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  */

#pragma once

#include <cctype> // isspace

namespace jive
{

namespace strings
{

template<typename ContainerT>
ContainerT SplitOnWhiteSpace(const std::string &input)
{
    ContainerT result;
    auto it = input.begin();
    auto spaceIt = it;

    while (true)
    {
        // Find the next occurrence of whitespace
        spaceIt = std::find_if(
            it,
            input.end(),
            [](std::string::const_reference character) -> bool
            {
                return isspace(character);
            });

        result.push_back(std::string(it, spaceIt));

        if (spaceIt == input.end())
        {
            // We didn't find any more whitespace.
            return result;
        }

        // Skip over the whitespace
        it = std::find_if(
            spaceIt,
            input.end(),
            [](std::string::const_reference character) -> bool
            {
                return !isspace(character);
            });

        if (it == input.end())
        {
            // We skipped over whitespace at the end of the string, which
            // requires an empty entry in the result.
            result.emplace_back("");
            return result;
        }
    }
}


/*
 * Creates one string constructed by concatenating InputIterator with token
 * inserted between. This is the complementary function to Split.
 *
 * @param InputIterator must dereference to a std::string or to any object that
 * can be a right hand operand to ostream::operator<<
 */
template<typename InputIterator, typename TokenT>
std::string Join(
    InputIterator first,
    InputIterator last,
    const TokenT &token)
{
    if (first == last)
    {
        return {};
    }

    // First get the size of the result string.
    size_t resultSize = 0;
    auto it = first;
    auto penultimate = last;
    --penultimate;

    size_t tokenSize = detail::GetTokenSize(token);
    while (it != penultimate)
    {
        resultSize += (it++)->size();
        resultSize += tokenSize;
    }

    resultSize += it->size();

    std::string result;
    result.reserve(resultSize);

    while (first != penultimate)
    {
        result += *first++;
        result += token;
    }

    // Add the last string without a final token.
    result += *first;
    return result;
}

template<typename TokenT>
std::string Join(
    std::initializer_list<std::string> initializerList,
    const TokenT &token)
{
    return Join(initializerList.begin(), initializerList.end(), token);
}


/** Concatenate avoids the creation of intermediate strings by using += on the
 ** same result.
 **/
#if __cplusplus >= 201703L
template<typename StringT, typename FirstT, typename... ArgsT>
StringT * Concatenate(StringT *ioResult, FirstT &&first, ArgsT&&... args)
{
    (*ioResult) += std::forward<FirstT>(first);
    if constexpr (sizeof...(ArgsT) > 0)
    {
        return Concatenate(ioResult, std::forward<ArgsT>(args)...);
    }
    else
    {
        return ioResult;
    }
}
#endif

} // end namespace strings

} // end namespace jive
