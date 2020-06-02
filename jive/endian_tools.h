/**
  * @file endian_tools.h
  *
  * @brief Templated functions for simple endian conversions.
  *
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 14 Nov 2017
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  */

#pragma once
#include <cstdint>
#include <cassert>
#include <type_traits>
#include <algorithm>

#ifdef __APPLE__
 /*
 * This is a simple compatibility shim to convert
 * BSD/Linux endian macros to the Mac OS X equivalents.
 * */

#include <libkern/OSByteOrder.h>

#define htobe16(x) OSSwapHostToBigInt16(x)
#define htole16(x) OSSwapHostToLittleInt16(x)
#define be16toh(x) OSSwapBigToHostInt16(x)
#define le16toh(x) OSSwapLittleToHostInt16(x)

#define htobe32(x) OSSwapHostToBigInt32(x)
#define htole32(x) OSSwapHostToLittleInt32(x)
#define be32toh(x) OSSwapBigToHostInt32(x)
#define le32toh(x) OSSwapLittleToHostInt32(x)

#define htobe64(x) OSSwapHostToBigInt64(x)
#define htole64(x) OSSwapHostToLittleInt64(x)
#define be64toh(x) OSSwapBigToHostInt64(x)
#define le64toh(x) OSSwapLittleToHostInt64(x)
#else // not __APPLE__
#include <endian.h>
#endif

namespace jive
{

namespace detail
{

union EndianTest
{
    char c[4];
    int32_t intValue;
};

inline constexpr const EndianTest endianTest = {{'\1', '\0', '\0', '\0'}};

} // end namespace detail


inline bool HostIsBigEndian()
{
    return detail::endianTest.intValue == 0x1000;
}


/*
 * The following templates choose the correct byte order conversion function
 * based on argument type. Note that there are 24 specializations for four
 * basic functions. T is any signed or unsigned integer with 8, 16, 32, or 64
 * bits, or any floating-point type.
 *
 *     T LittleEndianToHost(T)
 *     T BigEndianToHost(T)
 *     T HostToLittleEndian(T)
 *     T HostToBigEndian(T)
 *
 * Also provided are overloads for each function accepting void *. Because it is
 * impossible to deduce T when passing void * as the argument, the template
 * parameter must be passed explicitly.
 *
 *     T LittleEndianToHost<T>(void *);
 *     T BigEndianToHost<T>(void *);
 *     T HostToLittleEndian<T>(void *);
 *     T HostToBigEndian<T>(void *);
 */

template<typename T, uint8_t width>
struct EnableIntegralNotPointer
    :
    std::integral_constant<
        bool,
        sizeof(T) == width
        && std::is_integral<T>::value
        && !std::is_pointer<T>::value
    > {};

template<
    typename T,
    typename std::enable_if<
        EnableIntegralNotPointer<T, 2>::value, int>::type = 0>
T LittleEndianToHost(const T &value)
{
    return static_cast<T>(le16toh(value));
}

template<
    typename T,
    typename std::enable_if<
        EnableIntegralNotPointer<T, 4>::value, int>::type = 0>
T LittleEndianToHost(const T &value)
{
    return static_cast<T>(le32toh(value));
}

template<
    typename T,
    typename std::enable_if<
        EnableIntegralNotPointer<T, 8>::value, int>::type = 0>
T LittleEndianToHost(const T &value)
{
    return static_cast<T>(le64toh(value));
}

template<
    typename T,
    typename std::enable_if<
        EnableIntegralNotPointer<T, 2>::value, int>::type = 0>
T BigEndianToHost(const T &value)
{
    return static_cast<T>(be16toh(value));
}

template<
    typename T,
    typename std::enable_if<
        EnableIntegralNotPointer<T, 4>::value, int>::type = 0>
T BigEndianToHost(const T &value)
{
    return static_cast<T>(be32toh(value));
}

template<
    typename T,
    typename std::enable_if<
        EnableIntegralNotPointer<T, 8>::value, int>::type = 0>
T BigEndianToHost(const T &value)
{
    return static_cast<T>(be64toh(value));
}

template<
    typename T,
    typename std::enable_if<
        EnableIntegralNotPointer<T, 2>::value, int>::type = 0>
T HostToLittleEndian(const T &value)
{
    return static_cast<T>(htole16(value));
}

template<
    typename T,
    typename std::enable_if<
        EnableIntegralNotPointer<T, 4>::value, int>::type = 0>
T HostToLittleEndian(const T &value)
{
    return static_cast<T>(htole32(value));
}

template<
    typename T,
    typename std::enable_if<
        EnableIntegralNotPointer<T, 8>::value, int>::type = 0>
T HostToLittleEndian(const T &value)
{
    return static_cast<T>(htole64(value));
}

template<
    typename T,
    typename std::enable_if<
        EnableIntegralNotPointer<T, 2>::value, int>::type = 0>
T HostToBigEndian(const T &value)
{
    return static_cast<T>(htobe16(value));
}

template<
    typename T,
    typename std::enable_if<
        EnableIntegralNotPointer<T, 4>::value, int>::type = 0>
T HostToBigEndian(const T &value)
{
    return static_cast<T>(htobe32(value));
}

template<
    typename T,
    typename std::enable_if<
        EnableIntegralNotPointer<T, 8>::value, int>::type = 0>
T HostToBigEndian(const T &value)
{
    return static_cast<T>(htobe64(value));
}

template<typename T>
T ReverseByteOrder(const T &value)
{
    T convertedValue(value);
    char *convert = reinterpret_cast<char *>(&convertedValue);
    std::reverse(convert, convert + sizeof(T));
    return convertedValue;
}

template<
    typename T,
    typename std::enable_if<
        std::is_floating_point<T>::value, int>::type = 0>
T LittleEndianToHost(const T &value)
{
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    // Host is already little endian.
    return value;
#else
    // Host is big-endian
    return ReverseByteOrder(value);
#endif
}

template<
    typename T,
    typename std::enable_if<
        std::is_floating_point<T>::value, int>::type = 0>
T BigEndianToHost(const T &value)
{
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    // Host is already big endian.
    return value;
#else
    return ReverseByteOrder(value);
#endif

}

template<
    typename T,
    typename std::enable_if<
        std::is_floating_point<T>::value, int>::type = 0>
T HostToBigEndian(const T &value)
{
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    // Host is already big endian.
    return value;
#else
    return ReverseByteOrder(value);
#endif
}

template<
    typename T,
    typename std::enable_if<
        std::is_floating_point<T>::value, int>::type = 0>
T HostToLittleEndian(const T &value)
{
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    // Host is already little endian.
    return value;
#else
    return ReverseByteOrder(value);
#endif
}

/** void * conversions **/
template<typename T>
T LittleEndianToHost(void *data)
{
    return LittleEndianToHost(*static_cast<T *>(data));
}

template<typename T>
T BigEndianToHost(void *data)
{
    return BigEndianToHost(*static_cast<T *>(data));
}

template<typename T>
T HostToLittleEndian(void *data)
{
    return HostToLittleEndian(*static_cast<T *>(data));
}

template<typename T>
T HostToBigEndian(void *data)
{
    return HostToBigEndian(*static_cast<T *>(data));
}

/* These are defined as pass-through methods for single byte values.
 * These shouldn't be used if you know that the value is a single byte, but
 * they are here to allow flexibility in templated functions that accept both
 * single- and multi-byte values.
 */
inline uint8_t HostToBigEndian(const uint8_t &value)
{
    return value;
}

inline uint8_t BigEndianToHost(const uint8_t &value)
{
    return value;
}

inline uint8_t HostToLittleEndian(const uint8_t &value)
{
    return value;
}

inline uint8_t LittleEndianToHost(const uint8_t &value)
{
    return value;
}

inline int8_t HostToBigEndian(const int8_t &value)
{
    return value;
}

inline int8_t BigEndianToHost(const int8_t &value)
{
    return value;
}

inline int8_t HostToLittleEndian(const int8_t &value)
{
    return value;
}

inline int8_t LittleEndianToHost(const int8_t &value)
{
    return value;
}

} // end namespace jive
