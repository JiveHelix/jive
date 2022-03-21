/**
  * @file future.h
  * 
  * @brief Implementation of future features.
  * 
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 28 Mar 2022
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
**/

#pragma once

#include <type_traits>

#if __cplusplus <= 201703L
// C++17 and earlier does not have this convenience.

namespace std
{

template<typename T>
struct remove_cvref
{
    typedef std::remove_cv_t<std::remove_reference_t<T>> type;
};

template<typename T>
using remove_cvref_t = typename remove_cvref<T>::type;

} // end namespace std

#endif
