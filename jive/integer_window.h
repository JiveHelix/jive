/**
  * @file integer_window.h
  * 
  * @brief Stores a power-of-two length array of integers, and uses right shift
  * to compute the average from the sum.
  * 
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 29 Apr 2018
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  */

#pragma once

#include "jive/circular_index.h"
#include "jive/power.h"
#include "jive/averaging_window.h"

namespace jive
{

template<typename T, size_t N, typename Enable = void> class IntegerWindow;

/**
 ** Uses memoization to store running sum to speed the calculation of the
 ** average value.
 **
 ** @tparam T The type of the stored elements
 ** @tparam N Determins the element count as 2 ^ N.
 **/
template<typename T, size_t N>
class IntegerWindow<T, N, typename std::enable_if_t<std::is_integral<T>::value>>
    :
    public AveragingWindow<T, Power<size_t, N>(2)>
{
public:
    // Override GetAverage from the base class for more efficient integer
    // division.
    T GetAverage() const
    {
        return this->sum_ >> N;
    }
};

} // end namespace jive
