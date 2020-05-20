/**
  * @file eigen.h
  * 
  * @brief A few convenience functions for working with Eigen.
  * 
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 19 May 2020
  * Licensed under the MIT license. See LICENSE file.
**/
#pragma once

#include <type_traits>
#include "Eigen/Dense"

namespace jive
{

namespace detail
{

#if __cplusplus <= 201703L
// C++17 and earlier does not have this convenience.

template<typename T>
struct remove_cvref
{
    typedef std::remove_cv_t<std::remove_reference_t<T>> type;
};

template<typename T>
using remove_cvref_t = typename remove_cvref<T>::type;

#else
// C++20 already has it

template<typename T>
using remove_cvref_t = std::remove_cvref_t;

#endif
} // end namespace detail


/** Eigen 3.3.7 does not support brace initialization, and comma initialization
 ** is unsafe because it does not fail until runtime when Eigen discovers an
 ** incorrect number of arguments.
 **
 ** This helper allows comma initialization to be used as a one-liner, and
 ** provides a compile time check on the argument count.
 **/
template<int columns, int rows, typename T, typename ...Values>
Eigen::Matrix<detail::remove_cvref_t<T>, columns, rows>
EigenMatrix(T &&first, Values &&...values)
{
    static_assert(sizeof...(Values) + 1 == columns * rows);
    Eigen::Matrix<detail::remove_cvref_t<T>, columns, rows> result;
    ((result << std::forward<T>(first)), ..., std::forward<Values>(values));
    return result;
}

} // end namespace jive
