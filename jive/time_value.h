/**
  * @file time_value.h
  *
  * @brief A class for storing and manipulating time values.
  *
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 14 Nov 2017
  *
  * @copyright Jive Helix 2017 - 2020
  * Licensed under the MIT license. See LICENSE file.
  */

#pragma once

#include <cmath>
#include <chrono>
#include <ostream>
#include <ctime>

#ifdef _WIN32
// If winsock.h is included before winsock2.h, compiler errors abound.
// Make winsock2.h the default to save us from Microsoft's poor header hygiene.
// https://stackoverflow.com/a/9168850/15751056
#include <winsock2.h> // timeval
#else
#include <sys/time.h>
#endif

#include "jive/create_exception.h"

#define HAS_ROUND

#ifdef __aarch64__
#ifndef _GLIBCXX_USE_C99_MATH_TR1
#undef HAS_ROUND
#endif
#endif

#ifdef HAS_ROUND
#define ROUND(arg) std::round((arg))
#else
#define ROUND(arg) (arg)
#endif


namespace jive
{

CREATE_EXCEPTION(TimeValueError, std::runtime_error);

template<typename T>
using Seconds = typename std::chrono::duration<T>;

template<typename T>
using Milliseconds = typename std::chrono::duration<T, std::milli>;

template<typename T>
using Microseconds = typename std::chrono::duration<T, std::micro>;

template<typename T>
using Nanoseconds = typename std::chrono::duration<T, std::nano>;

using BaseDuration = Nanoseconds<int64_t>;


class TimeValue
{
public:
    using Type = BaseDuration::rep;

    // Construct initialized to zero.
    TimeValue()
        :
        nanoseconds_(0)
    {

    }

    template<typename T, long N, long D>
    explicit TimeValue(
        std::chrono::duration<T, std::ratio<N, D>> duration)
        :
        nanoseconds_(duration)
    {

    }

    // Initialize with floating point seconds
    template<
        typename T,
        typename = std::void_t<
            std::enable_if_t<
                std::is_arithmetic_v<T>
            >
        >
    >
    explicit TimeValue(T seconds)
        :
        nanoseconds_(Seconds<T>(seconds))
    {

    }

    TimeValue & SetToNow();

    template<typename T>
    T GetAsSeconds() const
    {
        // std::chrono::duration accounts for the conversion between
        // nanoseconds and seconds.

        static_assert(std::is_arithmetic_v<T>);

        if constexpr (std::is_integral_v<T>)
        {
            return static_cast<T>(
                ROUND(Seconds<double>(this->nanoseconds_).count()));
        }
        else
        {
            // The requested type is floating-point.
            return Seconds<T>(this->nanoseconds_).count();
        }
    }

    template<typename T>
    void SetAsSeconds(T seconds)
    {
        static_assert(std::is_arithmetic_v<T>);
        this->nanoseconds_ = Seconds<T>(seconds);
    }

    // Get/Set time represented as microseconds.
    template<typename T>
    T GetAsMicroseconds() const
    {
        static_assert(std::is_arithmetic_v<T>);

        if constexpr (std::is_integral_v<T>)
        {
            return static_cast<T>(
                ROUND(Microseconds<double>(this->nanoseconds_).count()));
        }
        else
        {
            // The requested type is floating-point.
            return Microseconds<T>(this->nanoseconds_).count();
        }
    }

    template<typename T>
    void SetAsMicroseconds(T microseconds)
    {
        static_assert(std::is_arithmetic_v<T>);
        this->nanoseconds_ = Microseconds<T>(microseconds);
    }

    TimeValue &AddOffset(const TimeValue &offset);
    TimeValue GetOffset(const TimeValue &offset) const;

    TimeValue &operator+=(const TimeValue &other);
    TimeValue operator+(const TimeValue &other) const;
    TimeValue &operator-=(const TimeValue &other);
    TimeValue operator-(const TimeValue &other) const;

    double operator/(const TimeValue &other) const
    {
        return this->GetAsSeconds<double>() / other.GetAsSeconds<double>();
    }

    bool operator==(const TimeValue &other) const;
    bool operator!=(const TimeValue &other) const;
    bool operator<(const TimeValue &other) const;
    bool operator<=(const TimeValue &other) const;
    bool operator>(const TimeValue &other) const;
    bool operator>=(const TimeValue &other) const;

    timeval GetAsTimeval() const;

    timespec GetAsTimespec() const;

    std::string GetAsIso8601() const;

    std::string GetAsIso8601WithColonTimeSeparator() const;

    std::string GetAsIso8601Precise(size_t decimalCount) const;

    static TimeValue GetNow();

    static TimeValue GetInterval(const TimeValue &startTime)
    {
        return TimeValue::GetNow() - startTime;
    }

    static double GetNowDouble();

    int64_t GetNanoseconds() const
    {
        return this->nanoseconds_.count();
    }

    BaseDuration GetDuration() const
    {
        return this->nanoseconds_;
    }

private:
    BaseDuration nanoseconds_;
};


std::ostream &operator<<(
    std::ostream &outputStream,
    const TimeValue &timeValue);


std::string GetNowIso8601();


} // end namespace jive


#undef max

namespace std
{
    // Overloading std::max and std::abs requires ignoring case requirements.
    // NOLINTNEXTLINE(readability-identifier-naming)
    inline jive::TimeValue max(
        const jive::TimeValue &first,
        const jive::TimeValue &second)
    {
        return (first > second) ? first : second;
    }

    // NOLINTNEXTLINE(readability-identifier-naming)
    inline jive::TimeValue abs(const jive::TimeValue &timeValue)
    {
        return jive::TimeValue(
            jive::BaseDuration(abs(timeValue.GetNanoseconds())));
    }
}

