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
std::ostream & StreamArgs(std::ostream &outputStream, const T &arg)
{
    return outputStream << arg << '\n';
}

template<typename T, typename... Args>
std::ostream & StreamArgs(
    std::ostream &outputStream,
    const T &arg,
    const Args&... args)
{
    outputStream << arg;
    return StreamArgs(outputStream, args...);
}

template<typename T>
std::ostream & StreamArgsFlush(std::ostream &outputStream, const T &arg)
{
    return outputStream << arg << std::endl;
}

template<typename T, typename... Args>
std::ostream & StreamArgsFlush(
    std::ostream &outputStream,
    const T &arg,
    const Args&... args)
{
    outputStream << arg;
    return StreamArgsFlush(outputStream, args...);
}

} // end namespace jive
