/**
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  */

#pragma once

// #define ENABLE_BLOCK_TIMER

#ifdef ENABLE_BLOCK_TIMER

#include "jive/standard_log.h"
#include "jive/time_value.h"

#define BLOCK_TIMER_LOG(...) STANDARD_LOG("BlockTimer", ##__VA_ARGS__)

namespace jive
{

class BlockTimer
{
public:
    BlockTimer(
        const std::string &fileName,
        const std::string &functionName,
        int lineNumber,
        const TimeValue &thresholdInterval,
        const std::string &message = "")
        :
        fileName_(fileName),
        functionName_(functionName),
        lineNumber_(lineNumber),
        thresholdInterval_(thresholdInterval),
        message_(message),
        startTime_(TimeValue::GetNow())
    {

    }

    ~BlockTimer()
    {
        TimeValue blockTime(TimeValue::GetNow() - this->startTime_);
        if (blockTime > this->thresholdInterval_)
        {
            if (this->message_.size())
            {
                BLOCK_TIMER_LOG(
                    this->fileName_, ":",
                    this->functionName_, ":",
                    this->lineNumber_ , " ",
                    blockTime.GetAsSeconds<double>(), " ",
                    this->message_);
            }
            else
            {
                BLOCK_TIMER_LOG(
                    this->fileName_, ":",
                    this->functionName_, ":",
                    this->lineNumber_ , " ",
                    blockTime.GetAsSeconds<double>());
            }
        }
    }

private:
    std::string fileName_;
    std::string functionName_;
    int lineNumber_;
    TimeValue thresholdInterval_;
    std::string message_;
    TimeValue startTime_;
};

} // end namespace jive

#define BLOCK_TIMER(thresholdInterval) \
    jive::BlockTimer blockTimer##__LINE__( \
        __FILE__, \
        __func__, \
        __LINE__, \
        thresholdInterval)

#define MESSAGE_BLOCK_TIMER(thresholdInterval, message) \
    jive::BlockTimer blockTimer##__LINE__( \
        __FILE__, \
        __func__, \
        __LINE__, \
        thresholdInterval, \
        message)
#else
#define BLOCK_TIMER(thresholdInterval)
#define MESSAGE_BLOCK_TIMER(thresholdInterval, message)
#endif
