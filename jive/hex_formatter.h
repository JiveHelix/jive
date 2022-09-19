/**
  * @file hex_formatter.h
  * 
  * @brief Tools for printing any object as bytes.
  * 
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 14 Nov 2017
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
**/

#pragma once

#undef min
#undef max

#include <iostream>
#include <iomanip>
#include <ostream>
#include <type_traits>
#include <cstdint>

#undef str
#include <sstream>

#include <cctype>
#include "jive/strings.h"
#include "jive/formatter.h"
#include "jive/preserve_format.h"


// A shortcut to cast single byte values to uint16_t to force operator<< to
// display the byte as a number rather than a char.
#define BYTE_CAST(byte) static_cast<uint16_t>(byte)

#define PRINT_BYTE(byte) std::setw(2) << BYTE_CAST(byte)

namespace jive
{

static const size_t ROW_WIDTH = 16;

inline
void GetPrintable(
    const void *voidData,
    size_t byteCount,
    char *outPrintable)
{
    const char *data = reinterpret_cast<const char *>(voidData);
    for (size_t i = 0; i < byteCount; ++i) 
    {
        if (isprint(data[i]))
        {
            outPrintable[i] = data[i];
        }
        else
        {
            outPrintable[i] = '.';
        }
    }

    outPrintable[byteCount] = '\0';
}


inline
std::ostream & PrintHex(
    std::ostream &outputStream,
    const void *voidData,
    size_t byteCount)
{
    PreserveFormat format(outputStream);

    const uint8_t *data = reinterpret_cast<const uint8_t *>(voidData);
    outputStream << std::hex << std::setfill('0');

    for (size_t i = 0; i < byteCount - 1; ++i) 
    {
        outputStream << PRINT_BYTE(data[i]) << " ";
    }
    
    // Output the last byte without a trailing space.
    outputStream << PRINT_BYTE(data[byteCount - 1]);

    return outputStream;
}


/**
 ** Each row of output contains the hexadecimal values followed by their
 ** printable ASCII values, or a '.' if it is not printable.
 **/
inline
std::ostream & PrintHexLinesWithAscii(
    std::ostream &outputStream,
    const void *voidData,
    size_t byteCount)
{
    PreserveFormat format(outputStream);

    const uint8_t *data = reinterpret_cast<const uint8_t *>(voidData);

    // Allow room for null terminator
    char printable[ROW_WIDTH + 1] = {0};

    // Output each row of ROW_WIDTH binary values
    for (
        size_t rowBeginIndex = 0;
        rowBeginIndex < byteCount;
        rowBeginIndex += ROW_WIDTH) 
    {
        outputStream
            << std::hex << "0x" << std::setfill('0') << std::setw(6)
            << std::right << rowBeginIndex << "   ";
        
        size_t charCount =
            std::min<size_t>(ROW_WIDTH, byteCount - rowBeginIndex);

        PrintHex(
            outputStream,
            data + rowBeginIndex,
            charCount);

        GetPrintable(data + rowBeginIndex, charCount, &printable[0]);
        
        // Pad the binary output to align any printable characters.
        size_t padCount = ROW_WIDTH - charCount;
        while (padCount--)
        {
            outputStream << "   ";
        }

        // Output the printable characters.
        outputStream << "    " << printable << "\n";
    }

    return outputStream;
}

inline
std::ostream & PrintHexLines(
    std::ostream &outputStream,
    const void *voidData,
    size_t byteCount)
{
    PreserveFormat format(outputStream);

    const uint8_t *data = reinterpret_cast<const uint8_t *>(voidData);
    
    outputStream << std::setfill(' ') << "Offset" << '\n'; 

    // Output each row of ROW_WIDTH binary values
    for (
        size_t rowBeginIndex = 0;
        rowBeginIndex < byteCount;
        rowBeginIndex += ROW_WIDTH) 
    {
        // Output the offset
        outputStream
            << std::dec << std::setfill(' ') << std::setw(6)
            << std::right << rowBeginIndex << "   ";
        
        // Output the row
        // When the last row has fewer than ROW_WIDTH bytes, make sure that we
        // do not read past bytecount.
        PrintHex(
            outputStream,
            data + rowBeginIndex,
            std::min<size_t>(ROW_WIDTH, byteCount - rowBeginIndex));

        outputStream << '\n';
    }

    return outputStream;
}


/** Recursively expand template functions to print a count of bytes known at
 ** compile time.
 **/

// End recursion without adding a trailing space
template<size_t byteCount, size_t index>
typename std::enable_if<(index == byteCount - 1)>::type
PrintBytes(std::ostream &outputStream, const uint8_t *data)
{
    outputStream << PRINT_BYTE(data[index]);
}

// Recursively calls PrintBytes<byteCount, index + 1>
template<size_t byteCount, size_t index>
typename std::enable_if<(index < byteCount - 1)>::type
PrintBytes(std::ostream &outputStream, const uint8_t *data)
{
    outputStream << PRINT_BYTE(data[index]) << " ";
    PrintBytes<byteCount, index + 1>(outputStream, data);
}

template<typename T>
std::ostream & PrintHex(std::ostream &outputStream, const T &value)
{
    PreserveFormat format(outputStream);

    outputStream << std::hex << std::setfill('0');

    PrintBytes<sizeof(T), 0>(
        outputStream,
        reinterpret_cast<const uint8_t *>(&value));

    return outputStream;
}

// struct for operator<< overload
template<typename T>
struct HexFormatter_
{
    HexFormatter_(const T &value): value(value) {}
    const T &value;
};

struct VoidHexFormatter_
{
    VoidHexFormatter_(const void *data, size_t byteCount)
        :
        data(data),
        byteCount(byteCount)
    {

    }
    
    const void *data;
    size_t byteCount;
};

// templated function to deduce T for HexFormatter_
template<typename T>
HexFormatter_<T> HexFormatter(const T &value)
{
    return HexFormatter_<T>(value);
}

template<typename T>
std::ostream & operator<<(
    std::ostream &outputStream,
    const HexFormatter_<T> &formatter)
{
    return PrintHex(outputStream, formatter.value);
}

inline
VoidHexFormatter_ HexFormatter(const void *data, size_t byteCount)
{
    return VoidHexFormatter_(data, byteCount);
}

inline
std::ostream & operator<<(
    std::ostream &outputStream,
    const VoidHexFormatter_ &formatter)
{
    return PrintHex(
        outputStream,
        formatter.data,
        formatter.byteCount);
}

struct ByteFormatter
{
    ByteFormatter(char byte)
        :
        byte(byte)
    {
    }

    char byte;
};

inline
std::ostream & operator<<(
    std::ostream &outputStream,
    const ByteFormatter &formatter)
{
    PreserveFormat format(outputStream);
    outputStream << PRINT_BYTE(formatter.byte);
    return outputStream;
}

template<typename T>
std::string GetAsHexFormattedString(const T &value)
{
    std::ostringstream outputStream;
    outputStream << HexFormatter(value);
    return outputStream.str();
}

inline
std::string GetAsHexFormattedString(const void *data, size_t byteCount)
{
    std::ostringstream outputStream;
    outputStream << HexFormatter(data, byteCount);
    return outputStream.str();
}

inline
std::string GetHexString(uint8_t byte)
{
    return FastFormatter<5>("0x%02hhX", byte);
}


} // namespace jive
