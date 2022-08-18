/**
  * @file time_value.cpp
  *
  * @brief A class for storing and manipulating time values.
  *
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 14 Nov 2017
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  */

#include "jive/time_value.h"

#include <iomanip>
#include <cstring>

#ifdef _WIN32
#include <ctime>
#endif

#include "jive/formatter.h"
#include "jive/strings.h"


namespace jive
{


inline constexpr const char *hyphenTimeFormat = "%Y-%m-%dT%H-%M-%S";
inline constexpr const char *colonTimeFormat = "%Y-%m-%dT%H:%M:%S";
inline constexpr const size_t maxFormattedLength = 32;


class MonotonicSystemClock
{
public:
    MonotonicSystemClock()
        :
        monotonicOffset_(
            std::chrono::system_clock::now().time_since_epoch()
            - std::chrono::steady_clock::now().time_since_epoch())
    {

    }

    TimeValue GetNow()
    {
        return TimeValue(
            std::chrono::steady_clock::now().time_since_epoch()
            + this->monotonicOffset_);
    }

private:
    BaseDuration monotonicOffset_;
};

MonotonicSystemClock monotonicSystemClock;


TimeValue TimeValue::GetNow()
{
    return monotonicSystemClock.GetNow();
}

double TimeValue::GetNowDouble()
{
    return TimeValue::GetNow().GetAsSeconds<double>();
}


TimeValue &TimeValue::SetToNow()
{
    *this = monotonicSystemClock.GetNow();
    return *this;
}


TimeValue &TimeValue::AddOffset(const TimeValue &offset)
{
    this->nanoseconds_ += offset.nanoseconds_;
    return *this;
}

TimeValue TimeValue::GetOffset(const TimeValue &offset) const
{
    TimeValue result(*this);
    return result.AddOffset(offset);
}

TimeValue &TimeValue::operator+=(const TimeValue &other)
{
    this->nanoseconds_ += other.nanoseconds_;
    return *this;
}

TimeValue TimeValue::operator+(const TimeValue &other) const
{
    TimeValue result(*this);
    result += other;
    return result;
}

TimeValue &TimeValue::operator-=(const TimeValue &other)
{
    this->nanoseconds_ -= other.nanoseconds_;
    return *this;
}

TimeValue TimeValue::operator-(const TimeValue &other) const
{
    TimeValue result(*this);
    result -= other;
    return result;
}

bool TimeValue::operator==(const TimeValue &other) const
{
    return this->nanoseconds_ == other.nanoseconds_;
}

bool TimeValue::operator!=(const TimeValue &other) const
{
    return !(*this == other);
}

bool TimeValue::operator<(const TimeValue &other) const
{
    return this->nanoseconds_ < other.nanoseconds_;
}

bool TimeValue::operator<=(const TimeValue &other) const
{
    return !(other < *this);
}

bool TimeValue::operator>(const TimeValue &other) const
{
    return (other < *this);
}

bool TimeValue::operator>=(const TimeValue &other) const
{
    return !(*this < other);
}

#ifdef _WIN32
#define LONG_CAST(arg) static_cast<long>(arg)
#else
#define LONG_CAST(arg) (arg)
#endif

timeval TimeValue::GetAsTimeval() const
{
    timeval tv;

    // Allow tv_sec to truncate.
    tv.tv_sec = LONG_CAST(
        this->nanoseconds_.count() / BaseDuration::period::den);

    double remainderNanoseconds = static_cast<double>(
        this->nanoseconds_.count() % BaseDuration::period::den);

    static constexpr double nanosecondsPerMicrosecond = 1000.0;

#ifdef _WIN32
    using suseconds_t = long;
#endif

    tv.tv_usec = static_cast<suseconds_t>(ROUND(
        remainderNanoseconds / nanosecondsPerMicrosecond));

    return tv;
}

timespec TimeValue::GetAsTimespec() const
{
    timespec ts;

    // Allow tv_sec to truncate.
    ts.tv_sec = this->nanoseconds_.count() / BaseDuration::period::den;

    int64_t remainderNanoseconds =
        this->nanoseconds_.count() % BaseDuration::period::den;

#ifdef _WIN32
    ts.tv_nsec = static_cast<long>(remainderNanoseconds);
#else
    ts.tv_nsec = remainderNanoseconds;
#endif

    return ts;
}

std::ostream &operator<<(
    std::ostream &outputStream,
    const TimeValue &timeValue)
{
    static constexpr auto microsecondWidth = 6;
    timeval tv = timeValue.GetAsTimeval();
    outputStream << std::dec << tv.tv_sec << "."
        << std::setfill('0') << std::setw(microsecondWidth) << tv.tv_usec;

    return outputStream;
}


namespace detail
{

void GetUtcTime(struct tm *result, time_t seconds)
{
#ifdef _WIN32
    errno_t valid = gmtime_s(result, &seconds);
    bool success = (valid == 0);
#else
    struct tm * valid = gmtime_r(&seconds, result);
    bool success = (valid != NULL);
#endif

    if (!success)
    {
        throw TimeValueError("Error converting time to UTC time.");
    }
}


template<bool useHyphenatedFormat>
std::string GetAsIso8601(time_t seconds)
{
    struct tm utcTime;
    GetUtcTime(&utcTime, seconds);
    std::string formattedTime;
    formattedTime.resize(maxFormattedLength);

    size_t count;

    if constexpr (useHyphenatedFormat)
    {
        count = strftime(
            &formattedTime[0],
            maxFormattedLength - 1,
            hyphenTimeFormat,
            &utcTime);
    }
    else
    {
        count = strftime(
            &formattedTime[0],
            maxFormattedLength - 1,
            colonTimeFormat,
            &utcTime);
    }

    if (count == 0)
    {
        throw TimeValueError("Time to string conversion failed.");
    }

    formattedTime[count] = 'Z';
    formattedTime.resize(count + 1);
    return formattedTime;
}

} // namespace detail

std::string TimeValue::GetAsIso8601() const
{
    return detail::GetAsIso8601<true>(this->GetAsSeconds<time_t>());
}

std::string TimeValue::GetAsIso8601WithColonTimeSeparator() const
{
    return detail::GetAsIso8601<false>(this->GetAsSeconds<time_t>());
}

std::string TimeValue::GetAsIso8601Precise(size_t decimalCount) const
{
    timespec ts = this->GetAsTimespec();
    struct tm utcTime;
    time_t seconds = ts.tv_sec;

    detail::GetUtcTime(&utcTime, seconds);

    std::string formattedTime;
    formattedTime.resize(maxFormattedLength);

    size_t count = strftime(
        &formattedTime[0],
        maxFormattedLength,
        hyphenTimeFormat,
        &utcTime);

    if (count == 0)
    {
        throw TimeValueError("Time to string conversion failed.");
    }

    formattedTime.resize(count);

    // Pad the nanoseconds to the left with zeros
    std::string nanosecondString = jive::FastFormatter<16>("%09ld", ts.tv_nsec);

    static constexpr size_t nanosecondsWidth = 9;

    if (decimalCount < nanosecondsWidth)
    {
        strings::Concatenate(
            &formattedTime,
            ".",
            nanosecondString.substr(0, decimalCount),
            "Z");
    }
    else
    {
        strings::Concatenate(
            &formattedTime,
            ".",
            nanosecondString,
            "Z");
    }

    return formattedTime;
}

std::string GetNowIso8601()
{
    return TimeValue::GetNow().GetAsIso8601();
}


} // end namespace jive
