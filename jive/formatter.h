/**
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  */

/**
 ** Example Usage:
 **
 ** For runtime checks on the format buffer. Allocates more space if necessary.
 **     std::string formatted = Formatter<64>("%d %s\n", 42, "is the answer");
 **
 ** Skips runtime checks. May segfault if the buffer size is insufficient.
 ** Prefer the safe version unless you have a good reason to need the speed.
 **     -- Bad. Possible segfault --
 **     std::string formatted =
 **         FastFormatter<10>("%d %s\n", 42, "is the answer");
 **
 **     -- Better because 18 is exactly the right size. --
 **     std::string formatted =
 **         FastFormatter<18>("%d %s\n", 42, "is the answer");
 **
 **     -- Best because we have some margin in case someone changes the format
 **         string. --
 **     std::string formatted =
 **         FastFormatter<32>("%d %s\n", 42, "is the answer");
 **/


/**
 ** Uses snprintf for safety. If charCount is insufficient, you will be
 ** penalized with an extra heap allocation and copy.
 **
 **/

#pragma once

// MSVC requires this reserved identifier.
// NOLINTNEXTLINE(bugprone-reserved-identifier)
#define __STD_WANT_LIB_EXT1__ 1

#include "jive/error.h"

#include <cstdarg>
#include <cstring>
#include <string>
#include <cerrno>
#include <stdexcept>

#undef FORMATTER_ATTRIBUTE
#ifndef _WIN32
#define FORMATTER_ATTRIBUTE __attribute__((format(printf, 1, 2)))
#else
#include "STDARG.H"
#define FORMATTER_ATTRIBUTE
#endif

namespace jive
{


template<size_t charCount>
std::string Formatter(const char *format, ...) FORMATTER_ATTRIBUTE;

template<size_t charCount>
std::string Formatter(const char *format, ...)
{
    std::string result;
    result.resize(charCount);

    va_list args;
    va_start(args, format);

    int formatResult =
        vsnprintf(
            &result[0],
            charCount,
            format,
            args);

    va_end(args);

    if (formatResult < 0)
    {
        throw std::system_error(SystemError(errno), "Formatting error");
    }

    size_t formattedCharCount = static_cast<size_t>(formatResult);

    if ((formattedCharCount + 1) > charCount)
    {
        // The formatted output was truncated.
        // Allocate more space, and do it again.
        result.resize(formattedCharCount + 1);

        va_list argsForSecondAttempt;
        va_start(argsForSecondAttempt, format);

        vsnprintf(
            &result[0],
            formattedCharCount + 1,
            format,
            argsForSecondAttempt);

        va_end(argsForSecondAttempt);
    }

    // resize to length smaller than capacity does not free/allocate memory.
    result.resize(formattedCharCount);

    // std::string provides a move constructor, so copy of return value will be
    // elided. 12.8 (32)
    return result;
}


/**
 ** No checks are done on the length of the formatted string.
 **
 ** The caller assumes responsibility for determining the maximum possible
 ** length of the formatted string.
 **
 **/
template<size_t charCount>
std::string FastFormatter(const char *format, ...) FORMATTER_ATTRIBUTE;

template<size_t charCount>
std::string FastFormatter(const char *format, ...)
{
    std::string result;
    result.resize(charCount);

    va_list args;
    va_start(args, format);

    // Use vsprintf instead of vsnprintf because it is faster.
#if __STDC_LIB_EXT1__ || defined _WIN32
    int formatResult =
        vsprintf_s(&result[0], charCount, format, args);
#else
    int formatResult =
        vsnprintf(&result[0], charCount, format, args);
#endif

    va_end(args);

    if (formatResult < 0)
    {
        return std::string("Formatting error: ") + SystemError(errno).message();
    }

    size_t formattedCharCount = static_cast<size_t>(formatResult);

#ifndef NDEBUG
    if (static_cast<unsigned int>(formattedCharCount) > charCount)
    {
        throw std::logic_error(
            "formatted length ("
            + std::to_string(formattedCharCount)
            + ") is greater than requested buffer size ("
            + std::to_string(charCount)
            + ")");
    }
#endif

    result.resize(formattedCharCount);

    // std::string provides a move constructor, so copy of return value will be
    // elided. 12.8 (32)
    return result;
}


template<size_t N>
constexpr size_t GetFormatterCount(const char (&format)[N])
{
    bool lastWasFormatter = false;
    size_t count = 0;

    for (auto i = 0U; i < N; ++i)
    {
        if (format[i] == '%')
        {
            if (lastWasFormatter)
            {
                // This '%' was escaped, and does not count.
                // Uncount the previous '%'
                --count;
                lastWasFormatter = false;
            }
            else
            {
                // This is a new '%'
                ++count;
                lastWasFormatter = true;
            }
        }
        else
        {
            lastWasFormatter = false;
        }
    }

    return count;
}

template<typename... Types>
constexpr size_t GetArgCount(Types&&...)
{
    return sizeof...(Types);
}

} // end namespace jive

#if 0
// example static_assert to check argument count of runtime format strings.

#define DEFAULT_FORMATTED_LENGTH 256

#define FORMATTER(format, ...) \
    jive::Formatter<DEFAULT_FORMATTED_LENGTH>(format, ##__VA_ARGS__); \
    static_assert( \
        jive::GetFormatterCount(format) == jive::GetArgCount(__VA_ARGS__), \
        "Argument count does not match the format string.")


#define FAST_FORMATTER(count, format, ...) \
    jive::FastFormatter<count>(format, ##__VA_ARGS__); \
    static_assert( \
        jive::GetFormatterCount(format) == jive::GetArgCount(__VA_ARGS__), \
        "Argument count does not match the format string.")
#endif
