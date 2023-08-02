/**
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright 2011-2020 Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  */

#pragma once


#undef min
#undef max
#include <algorithm>

namespace jive
{

namespace strings
{

namespace detail
{

inline size_t GetTokenSize(const std::string &token)
{
    return token.size();
}


inline size_t GetTokenSize(char)
{
    return 1;
}

/*  The input str will be split on every occurrence of token.  The returned
 *  ContainerT of strings does not contain any instance of token.
 * @param limit The max count of tokens to parse.
 * @ return If limit is less than the count of tokens in the string, the last
 * string in the return ContainerT will contain the unprocessed portion of the
 * stream. Use limit=-1 to parse every occurence of token.
 */
template<typename ContainerT, typename TokenT>
ContainerT SplitToContainer(
    const std::string& input,
    const TokenT &token,
    int limit = -1)
{
    std::string::size_type position = 0;
    size_t tokenSize = GetTokenSize(token);

    if (tokenSize == 0)
    {
        throw std::invalid_argument("token must have non-zero length.");
    }

    ContainerT result;
    std::string::size_type tokenIndex;

    for (int i = 0; i != limit; i++)
    {
        tokenIndex = input.find(token, position);

        if (tokenIndex == std::string::npos)
        {
            break;
        }

        result.emplace_back(input.substr(position, tokenIndex - position));
        position = tokenIndex + tokenSize;
    }

    result.emplace_back(input, position);
    return result;
}


/**
 ** Modifiy the length of inString to the length returned by strlen.
 **/
inline
void ResizeToStrlen(std::string &inString)
{
    size_t byteCount = std::min(inString.size(), strlen(inString.c_str()));
    inString.resize(byteCount);
}

} // end namespace detail

} // end namespace strings

} // end namespace jive
