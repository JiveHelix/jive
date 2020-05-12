/**
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright 2011-2020 Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  */

#pragma once

#include <type_traits>

namespace jive
{

namespace io
{

namespace detail
{

template<typename T>
struct IsArithmeticAndNotAPointer
    :
    std::integral_constant<
        bool,
        !std::is_pointer<T>::value
        && std::is_arithmetic<T>::value
    > {};

/*
 * Enabler for generic Write/Read that can serialize/deserialize arbitrary
 * objects.
 * 
 *  1. All arithmetic types except pointers are allowed.
 *  2. std::string is allowed
 *  3. Allows any standard layout struct. (Even if it contains pointers, so be
 *  careful.)
 *  4. Allows char[], but not char * or const char *
 */
template<typename T>
struct EnableBinaryIo
    :
    std::integral_constant<
        bool,
        (IsArithmeticAndNotAPointer<T>::value
        || std::is_same<T, std::string>::value
        || std::is_standard_layout<T>::value)
        && !std::is_same<typename std::remove_const<T>::type, char *>::value
    > {};

} // end namespace detail

} // end namespace io

} // end namespace jive
