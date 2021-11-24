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

#elif defined(_WIN32)

#define NOMINMAX
#include <windows.h>

#if BYTE_ORDER == LITTLE_ENDIAN

#include <stdlib.h>
#define htobe16(x) _byteswap_ushort(x)
#define htole16(x) (x)
#define be16toh(x) _byteswap_ushort(x)
#define le16toh(x) (x)

#define htobe32(x) _byteswap_ulong(x)
#define htole32(x) (x)
#define be32toh(x) _byteswap_ulong(x)
#define le32toh(x) (x)

#define htobe64(x) _byteswap_uint64(x)
#define htole64(x) (x)
#define be64toh(x) _byteswap_uint64(x)
#define le64toh(x) (x)

#else // BYTE_ORDER != LITTLE_ENDIAN
#error Byte order not supported.
#endif

#else // not __APPLE__ and not _WIN32
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
        EnableIntegralNotPointer<T, 1>::value, int>::type = 0>
T LittleEndianToHost(const T &value)
{
    return value;
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
        EnableIntegralNotPointer<T, 1>::value, int>::type = 0>
T BigEndianToHost(const T &value)
{
    return value;
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
        EnableIntegralNotPointer<T, 1>::value, int>::type = 0>
T HostToLittleEndian(const T &value)
{
    return value;
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

template<
    typename T,
    typename std::enable_if<
        EnableIntegralNotPointer<T, 1>::value, int>::type = 0>
T HostToBigEndian(const T &value)
{
    return value;
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


namespace detail
{

    struct ToBigEndian
    {
        template<typename T>
        T operator()(T value)
        {
            return HostToBigEndian(value);
        }
    };

    struct FromBigEndian
    {
        template<typename T>
        T operator()(T value)
        {
            return BigEndianToHost(value);
        }
    };

    struct ToLittleEndian
    {
        template<typename T>
        T operator()(T value)
        {
            return HostToLittleEndian(value);
        }
    };

    struct FromLittleEndian
    {
        template<typename T>
        T operator()(T value)
        {
            return LittleEndianToHost(value);
        }
    };

} // end namespace detail


template<
    typename Operation,
    typename InputIterator,
    typename OutputIterator>
void RangeSwap(
    InputIterator inputBegin,
    InputIterator inputEnd,
    OutputIterator outputBegin)
{
    if constexpr (1 == sizeof(std::remove_pointer_t<InputIterator>))
    {
        // Can't swap single byte values.
        if (outputBegin != inputBegin)
        {
            // only copy is necessary.
            std::copy(inputBegin, inputEnd, outputBegin);
        }

        return;
    }

    while (inputBegin != inputEnd)
    {
        *outputBegin = Operation{}(*inputBegin);
        ++outputBegin;
        ++inputBegin;
    }
}


template<typename InputIterator, typename OutputIterator>
void HostToBigEndian(
    InputIterator inputBegin,
    InputIterator inputEnd,
    OutputIterator outputBegin)
{

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__

    // Host is already big-endian.

    if (outputBegin != inputBegin)
    {
        // only copy is necessary
        std::copy(inputBegin, inputEnd, outputBegin);
    }

#else

    RangeSwap<detail::ToBigEndian>(inputBegin, inputEnd, outputBegin);

#endif

}

template<typename InputIterator, typename OutputIterator>
void BigEndianToHost(
    InputIterator inputBegin,
    InputIterator inputEnd,
    OutputIterator outputBegin)
{

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__

    // Host is already big-endian.

    if (outputBegin != inputBegin)
    {
        // only copy is necessary
        std::copy(inputBegin, inputEnd, outputBegin);
    }

#else

    RangeSwap<detail::FromBigEndian>(inputBegin, inputEnd, outputBegin);

#endif

}

template<typename InputIterator, typename OutputIterator>
void HostToLittleEndian(
    InputIterator inputBegin,
    InputIterator inputEnd,
    OutputIterator outputBegin)
{

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

    // Host is already little-endian.

    if (outputBegin != inputBegin)
    {
        // only copy is necessary
        std::copy(inputBegin, inputEnd, outputBegin);
    }

#else

    RangeSwap<detail::ToLittleEndian>(inputBegin, inputEnd, outputBegin);

#endif

}

template<typename InputIterator, typename OutputIterator>
void LittleEndianToHost(
    InputIterator inputBegin,
    InputIterator inputEnd,
    OutputIterator outputBegin)
{

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

    // Host is already little-endian.

    if (outputBegin != inputBegin)
    {
        // only copy is necessary
        std::copy(inputBegin, inputEnd, outputBegin);
    }

#else

    RangeSwap<detail::FromLittleEndian>(inputBegin, inputEnd, outputBegin);

#endif
}


template<
    typename T,
    typename std::enable_if_t<std::is_array_v<T>, int> = 0
>
void HostToBigEndian(const T &input, T &output)
{
    HostToBigEndian(
        Begin(input),
        End(input),
        Begin(output));
}

template<
    typename T,
    typename std::enable_if_t<std::is_array_v<T>, int> = 0
>
void BigEndianToHost(const T &input, T &output)
{
    BigEndianToHost(
        Begin(input),
        End(input),
        Begin(output));
}

template<
    typename T,
    typename std::enable_if_t<std::is_array_v<T>, int> = 0
>
void HostToLittleEndian(const T &input, T &output)
{
    HostToLittleEndian(
        Begin(input),
        End(input),
        Begin(output));
}

template<
    typename T,
    typename std::enable_if_t<std::is_array_v<T>, int> = 0
>
void LittleEndianToHost(const T &input, T &output)
{
    LittleEndianToHost(
        Begin(input),
        End(input),
        Begin(output));
}


} // end namespace jive
