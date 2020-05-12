/**
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  */

#pragma once

#include <cerrno>
#include <sstream>
#include <string>
#include <cstring>

namespace jive
{

template<typename Exception>
Exception ErrnoException(const std::string &message, int errorNumber = 0)
{
    std::ostringstream outputStream(
        message,
        std::ostringstream::ate);

    if (errorNumber > 0)
    {
        outputStream << ", errno=" << errorNumber << ": "
            << std::strerror(errorNumber);
    }

    return Exception(outputStream.str());
}

} // end namespace jive
