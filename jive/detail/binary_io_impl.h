/**
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright 2011-2020 Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  */

#pragma once

namespace jive
{

namespace io
{

template<
    typename T,
    typename std::enable_if_t<detail::EnableBinaryIo<T>::value, int>>
T Read(std::istream &inputStream)
{
    T value;
    inputStream.read(reinterpret_cast<char *>(&value), sizeof(T));
    return value;
}

/** Read specialization for std::string **/
template<>
inline std::string Read<std::string>(std::istream &inputStream)
{
    int stringLength = inputStream.get();

    if (stringLength < 0)
    {
        throw BinaryIoError("Failed to extract string.");
    }

    size_t resultLength = static_cast<size_t>(stringLength);
    char buffer[256]{};
    inputStream.read(&buffer[0], stringLength);
    return std::string(&buffer[0], resultLength);
}

template<
    typename T,
    typename std::enable_if_t<detail::EnableBinaryIo<T>::value, int>>
void Write(std::ostream &outputStream, const T &value)
{
    outputStream.write(reinterpret_cast<const char *>(&value), sizeof(T));
}

/** Write specialization for std::string **/
template<>
inline void Write<std::string>(
    std::ostream &outputStream,
    const std::string &value)
{
    static constexpr auto maximumSize = std::numeric_limits<uint8_t>::max();
    const auto valueSize = value.size();

    if (valueSize > maximumSize)
    {
        throw std::length_error("String length is limited to 256 characters");
    }

    uint8_t stringSize = static_cast<uint8_t>(valueSize);
    outputStream.put(static_cast<char>(stringSize));
    outputStream.write(value.c_str(), stringSize);
}

template<
    typename T,
    typename std::enable_if_t<detail::EnableBinaryIo<T>::value, int>>
T Read(const ReadFunction &readFunction)
{
    T value;
    readFunction(&value, sizeof(T), 1);
    return value;
}

template<
    typename T,
    typename std::enable_if_t<detail::EnableBinaryIo<T>::value, int>>
void Write(const WriteFunction &writeFunction, const T &value)
{
    writeFunction(&value, sizeof(T), 1);
}

template<>
inline std::string Read<std::string>(const ReadFunction &readFunction)
{
    uint8_t stringSize;
    readFunction(&stringSize, 1, 1);
    char buffer[256]{};
    readFunction(&buffer[0], 1, stringSize);
    return std::string(&buffer[0], stringSize);
}

template<>
inline void Write<std::string>(
    const WriteFunction &writeFunction,
    const std::string &value)
{
    static constexpr auto maximumSize = std::numeric_limits<uint8_t>::max();
    const auto valueSize = value.size();

    if (valueSize > maximumSize)
    {
        throw std::length_error("String length is limited to 255 characters");
    }

    uint8_t stringSize = static_cast<uint8_t>(valueSize);
    writeFunction(&stringSize, 1, 1);
    writeFunction(value.c_str(), 1, stringSize);
}


/*
 * @brief Advance the readFunction
 *
 * @param readFunction The function object backed by a file, pipe, or stream
 * @param byteCount The number of bytes to skip.
 * @remark Thread-safe because dummyBytes is never read.
 *
 */
inline void Skip(const ReadFunction &readFunction, size_t byteCount)
{
    static uint8_t dummyBytes[256];
    while (byteCount > 0)
    {
        // Limit each loop count to available dummyBytes
        size_t loopCount = byteCount < 256 ? byteCount : 256;
        readFunction(&dummyBytes, 1, loopCount);
        byteCount -= loopCount;
    }
}


template<typename CountType>
void WriteString(
    std::ostream &outputStream,
    const std::string_view &longString)
{
    static constexpr auto maxCharCount =
        std::numeric_limits<CountType>::max();

    size_t charCount = longString.size();

    if (charCount > maxCharCount)
    {
        throw std::length_error(
            "String length is limited to " + std::to_string(maxCharCount));
    }

    Write(outputStream, static_cast<CountType>(charCount));

    outputStream.write(
        longString.data(),
        static_cast<signed>(charCount));
}


template<typename CountType>
std::string ReadString(std::istream &inputStream)
{
    CountType charCount = Read<CountType>(inputStream);

    if (inputStream.gcount() != sizeof(CountType))
    {
        throw BinaryIoError("Failed to extract charCount");
    }

    std::string result;
    result.resize(charCount);
    inputStream.read(&result[0], static_cast<std::streamsize>(charCount));
    return result;
}


} // end namespace io

} // end namespace jive
