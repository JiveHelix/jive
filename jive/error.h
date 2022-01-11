/**
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  */

#pragma once

#include <cerrno>

#undef str
#include <sstream>

#include <string>
#include <cstring>
#include <system_error>

namespace jive
{


inline std::error_code SystemError(int errorNumber)
{
    return std::error_code(errorNumber, std::system_category());
}


[[deprecated("Use std::error_code directly")]]
inline std::string StringError(int errorNumber)
{
    return SystemError(errorNumber).message();
}


template<typename Exception>
[[deprecated("Subclass std::system_error instead.")]]
Exception ErrnoException(const std::string &message, int errorNumber = 0)
{
    std::ostringstream outputStream(
        message,
        std::ostringstream::ate);

    if (errorNumber > 0)
    {
        outputStream << ", errno=" << errorNumber << ": "
            << StringError(errorNumber);
    }

    return Exception(outputStream.str());
}

} // end namespace jive
