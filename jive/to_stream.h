/**
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  */

#pragma once
#include <ostream>
#include <type_traits>
#include <iostream>

namespace jive
{

template<typename T>
std::ostream & ToStream(std::ostream &outputStream, const T &arg)
{
    if constexpr (std::is_pointer_v<T>)
    {
        assert(arg != nullptr);
    }

    return outputStream << arg << '\n';
}

template<typename T, typename... Args>
std::ostream & ToStream(
    std::ostream &outputStream,
    const T &arg,
    const Args&... args)
{
    if constexpr (std::is_pointer_v<T>)
    {
        assert(arg != nullptr);
    }

    outputStream << arg;
    return ToStream(outputStream, args...);
}

template<typename T>
std::ostream & ToStreamFlush(std::ostream &outputStream, const T &arg)
{
    return outputStream << arg << std::endl;
}

template<typename T, typename... Args>
std::ostream & ToStreamFlush(
    std::ostream &outputStream,
    const T &arg,
    const Args&... args)
{
    outputStream << arg;
    return ToStreamFlush(outputStream, args...);
}

} // end namespace jive
