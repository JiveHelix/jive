/**
  * @file binary_io.h
  *
  * @brief Convenience functions for reading and writing values as binary data.
  *
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright 2011-2020 Jive Helix
  *
  * Licensed under the MIT license. See LICENSE file.
  */

#pragma once

#include <istream>
#include <ostream>
#include <string>
#include <cstdint>
#include <functional>
#include <limits>

#include <type_traits>
#include "jive/create_exception.h"

#include "jive/detail/binary_io_detail.h"

namespace jive
{

namespace io
{


CREATE_EXCEPTION(BinaryIoError, std::runtime_error);


/**
  * Read and Write function objects that are already associated with an open
  * file, descriptor, or stream. Both read/write function objects process
  * the exact count requested or throw.
  * ReadFunction accepts target data as the first argument. This is where the
  * bytes will be stored as they are read from the file/stream.
  *
  * WriteFunction accepts source data as the first argument. These bytes will
  * be written out to the file/stream.
  */


/**
  * @param target data (where to store the read bytes)
  * @param itemSize
  * @param itemCount
  */
using ReadFunction = std::function<void (void * const, size_t, size_t)>;


/**
  * @param source data
  * @param itemSize
  * @param itemCount
  */
using WriteFunction = std::function<void (const void * const, size_t, size_t)>;


template<
    typename T,
    typename std::enable_if_t<detail::EnableBinaryIo<T>::value, int> = 0>
T Read(std::istream &inputStream);


template<
    typename T,
    typename std::enable_if_t<detail::EnableBinaryIo<T>::value, int> = 0>
T Read(const ReadFunction &readFunction);


template<
    typename T,
    typename std::enable_if_t<detail::EnableBinaryIo<T>::value, int> = 0>
void Write(std::ostream &outputStream, const T &value);


template<
    typename T,
    typename std::enable_if_t<detail::EnableBinaryIo<T>::value, int> = 0>
void Write(const WriteFunction &writeFunction, const T &value);


/*
 * @brief Advance the readFunction
 *
 * @param readFunction The function object backed by a file, pipe, or stream
 * @param byteCount The number of bytes to skip.
 * @remark Thread-safe because dummyBytes is never read.
 *
 */
void Skip(const ReadFunction &readFunction, size_t byteCount);


template<typename T>
void Skip(std::istream &inputStream)
{
    inputStream.seekg(sizeof(T), std::istream::cur);
}


/*
 * Serialize strings with CountType other than uint8_t
 *
 */

template<typename CountType = uint32_t>
void WriteString(
    std::ostream &outputStream,
    const std::string_view &longString);


template<typename CountType = uint32_t>
std::string ReadString(std::istream &inputStream);


} // end namespace io


} // end namespace jive


#include "jive/detail/binary_io_impl.h"
