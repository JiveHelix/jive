/**
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  */

#pragma once

#define __STD_WANT_LIB_EXT1__ 1
#include <cerrno>
#include <sstream>
#include <string>
#include <cstring>

namespace jive
{


inline std::string StringError(int errorNumber)
{
    char buffer[64];

#ifdef _WIN32
    errno_t success = strerror_s(buffer, sizeof(buffer), errorNumber);
#else
    int success = strerror_r(errorNumber, buffer, sizeof(buffer));
#endif

    if (success != 0)
    {
        return std::string("Unknown error: ") + std::to_string(errorNumber);
    }

    return std::string{&buffer[0]};
}


template<typename Exception>
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
