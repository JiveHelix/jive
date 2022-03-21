/**
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright 2020 Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  */

#pragma once

#include <limits>

template<typename T, typename Target = T>
struct CastLimits
{
    static constexpr Target Min()
    {
        return static_cast<Target>(std::numeric_limits<T>::min());
    }

    static constexpr Target Max()
    {
        return static_cast<Target>(std::numeric_limits<T>::max());
    }
};
