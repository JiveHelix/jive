/**
  * @file strings.h
  *
  * @brief String manipulation utilities.
  *
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright 2011-2018 Jive Helix
  *
  * Licensed under the MIT license. See LICENSE file.
  */

#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <initializer_list>
#include <numeric>
#include <cassert>

#undef str
#include <sstream>

#include <cstring>
#include <cstdarg>
#include <cctype>

#include "jive/detail/strings_detail.h"


namespace jive
{

namespace strings
{

/**
 ** Return a string with length up to any optional null character, or the full
 ** length of the input array if there is no null character.
 **/
template<size_t N>
std::string Make(const char (&bytes)[N])
{
    std::string result(bytes, N);
    detail::ResizeToStrlen(result);
    return result;
}

/**
 ** Return a string with length up to any optional null character, or
 ** maxByteCount if there is no null character.
 **/
inline
std::string Make(const char *inArray, size_t maxByteCount)
{
    std::string result(inArray, maxByteCount);
    detail::ResizeToStrlen(result);
    return result;
}

/**
 ** Remove all leading and trailing occurrences of specified
 ** characters.
 ** Defaults to whitespace characaters.
 **/
inline
std::string Trim(
    const std::string &inString,
    const std::string &remove = " \t\r\n")
{
    const auto begin = inString.find_first_not_of(remove);
    if (begin == std::string::npos)
    {
        // All unwanted
        return "";
    }

    const auto end = inString.find_last_not_of(remove);
    const auto count = end - begin + 1;
    return inString.substr(begin, count);
}


template<size_t count>
bool CharArrayEquals(
    const char (&charArray)[count],
    const char (&compare)[count])
{
    return std::equal(&compare[0], &compare[0] + count, &charArray[0]);
}


template<typename TokenT>
std::vector<std::string> Split(
    const std::string& input,
    const TokenT &token,
    int limit = -1)
{
    return detail::SplitToContainer<std::vector<std::string>, TokenT>(
        input,
        token,
        limit);
}


template<typename ContainerT = std::vector<std::string>>
ContainerT SplitOnWhiteSpace(const std::string &input);


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
    const TokenT &token);

template<typename TokenT>
std::string Join(
    std::initializer_list<std::string> initializerList,
    const TokenT &token);


/** Concatenate avoids the creation of intermediate strings by using += on the
 ** same result.
 **/
#if __cplusplus >= 201703L
template<typename StringT, typename FirstT, typename... ArgsT>
StringT * Concatenate(StringT *ioResult, FirstT &&first, ArgsT&&... args);
#endif


/* Test whether a string is all digits */
inline
bool AllOfDigits(const std::string& s)
{
    return std::all_of(
        s.begin(),
        s.end(),
        [](char c)->bool { return isdigit(c); });
}

/* Test whether a string is all alpha numeric */
inline
bool AllOfAlphaNumerics(const std::string& s)
{
    return std::all_of(
        s.begin(),
        s.end(),
        [](char c)->bool { return isalnum(c); });
}

/* Test whether a string is all alpha */
inline
bool AllOfAlphas(const std::string& s)
{
    return std::all_of(
        s.begin(),
        s.end(),
        [](char c)->bool { return isalpha(c); });
}


inline std::string Upper(std::string value)
{
    std::transform(
        value.begin(),
        value.end(),
        value.begin(),
        [](unsigned char c)
        {
            return std::toupper(c);
        });

    return value;
}

inline std::string Lower(std::string value)
{
    std::transform(
        value.begin(),
        value.end(),
        value.begin(),
        [](unsigned char c)
        {
            return std::tolower(c);
        });

    return value;
}


} // end namespace strings

} // end namespace jive

#include "jive/detail/strings_impl.h"
