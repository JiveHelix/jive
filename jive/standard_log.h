/**
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  */

#pragma once

/**
 ** The macros STANDARD_LOG and DEBUG_LOG are not meant to be used directly.
 **
 ** Use them to define your own macros like this:
 ** #define RADAR_LOG(...) STANDARD_LOG("Radar", ##__VA_ARGS__)
 ** #define RADAR_DEBUG_LOG(...) DEBUG_LOG("Radar debug", ##__VA_ARGS__)
 **
 **/

#include <iostream>
#include "jive/to_stream.h"
#include "jive/time_value.h"

#ifndef TIME_STAMP_PRECISION
#define TIME_STAMP_PRECISION 3
#endif

#define STANDARD_LOG(logName, ...) \
    jive::ToStream( \
        std::cout, \
        jive::TimeValue::GetNow().GetAsIso8601Precise(TIME_STAMP_PRECISION), \
        " [" logName "] ", \
        ##__VA_ARGS__)

#define DEBUG_LOG(logName, ...) \
    jive::ToStreamFlush( \
        std::cout, \
        jive::TimeValue::GetNow().GetAsIso8601Precise(TIME_STAMP_PRECISION), \
        " [" logName "] ", \
        __func__, \
        ":", \
        std::dec, \
        __LINE__, \
        ": ", \
        ##__VA_ARGS__)
