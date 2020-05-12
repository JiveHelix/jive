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

#include <cmath>
#include <iomanip>
#include <cstring>

#include "jive/formatter.h"
#include "jive/strings.h"


namespace jive
{


inline constexpr const char *hyphenTimeFormat = "%Y-%m-%dT%H-%M-%S";
inline constexpr const char *colonTimeFormat = "%Y-%m-%dT%H:%M:%S";
inline constexpr const size_t maxFormattedLength = 32;


class MonotonicSystemClock_
{
public:
    MonotonicSystemClock_()
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

MonotonicSystemClock_ monotonicSystemClock_;


TimeValue TimeValue::GetNow()
{
    return monotonicSystemClock_.GetNow();
}

double TimeValue::GetNowDouble()
{
    return TimeValue::GetNow().GetAsSeconds<double>();
}


TimeValue &TimeValue::SetToNow()
{
    *this = monotonicSystemClock_.GetNow();
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

timeval TimeValue::GetAsTimeval() const
{
    timeval tv;

    // Allow tv_sec to truncate.
    tv.tv_sec = static_cast<std::time_t>(
        this->nanoseconds_.count() / BaseDuration::period::den);
    
    int64_t remainderNanoseconds =
        this->nanoseconds_.count() % BaseDuration::period::den;

    tv.tv_usec =
        static_cast<suseconds_t>(std::round(remainderNanoseconds / 1.0e3));

    return tv;
}

timespec TimeValue::GetAsTimespec() const
{
    timespec ts;

    // Allow tv_sec to truncate.
    ts.tv_sec = static_cast<std::time_t>(
        this->nanoseconds_.count() / BaseDuration::period::den);
    
    int64_t remainderNanoseconds =
        this->nanoseconds_.count() % BaseDuration::period::den;

    ts.tv_nsec = remainderNanoseconds;

    return ts;
}

std::ostream &operator<<(
    std::ostream &outputStream,
    const TimeValue &timeValue)
{
    timeval tv = timeValue.GetAsTimeval();
    outputStream << std::dec << tv.tv_sec << "."
        << std::setfill('0') << std::setw(6) << tv.tv_usec;

    return outputStream;
}

std::string TimeValue::GetAsIso8601_(const char *timeFormat) const
{
    auto seconds = this->GetAsSeconds<time_t>();

    struct tm utcTime;
    struct tm * result = gmtime_r(&seconds, &utcTime);

    if (!result)
    {
        throw TimeValueError("Error converting time to UTC time.");
    }

    std::string formattedTime;
    formattedTime.resize(maxFormattedLength);
    size_t count = strftime(
        &formattedTime[0],
        maxFormattedLength - 1,
        timeFormat,
        &utcTime); 

    if (count == 0)
    {
        throw TimeValueError("Time to string conversion failed.");
    }
    formattedTime[count] = 'Z';
    formattedTime.resize(count + 1);
    return formattedTime;
}

std::string TimeValue::GetAsIso8601() const
{
    return this->GetAsIso8601_(hyphenTimeFormat);
}

std::string TimeValue::GetAsIso8601WithColonTimeSeparator() const
{
    return this->GetAsIso8601_(colonTimeFormat);
}

std::string TimeValue::GetAsIso8601Precise(size_t decimalCount) const
{
    timespec ts = this->GetAsTimespec();
    struct tm utcTime;
    time_t seconds = ts.tv_sec;
    struct tm * result = gmtime_r(&seconds, &utcTime);

    if (!result)
    {
        throw TimeValueError("Error converting time to UTC time.");
    }

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
    std::string nanosecondString = jive::FastFormatter<10>("%09ld", ts.tv_nsec);

    if (decimalCount < 9)
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
