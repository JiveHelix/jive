/**
  * @file id_bytes.h
  *
  * @brief Wrap a templated number of bytes in a struct to use as a marker in
  * binary files.
  *
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright 2017-2020 Jive Helix
  *
  * Licensed under the MIT license. See LICENSE file.
  */

#pragma once

#include <ostream>
#include <utility>

namespace jive
{

template<size_t count>
struct IdBytes
{
    template<size_t ...Index>
    bool ComparesEqual(
        const IdBytes &other,
        std::index_sequence<Index...>) const
    {
        return ((this->bytes[Index] == other.bytes[Index]) && ...);
    }

    bool operator==(const IdBytes &other) const
    {
        return this->ComparesEqual(
            other,
            std::make_index_sequence<count>{});
    }

    bool operator!=(const IdBytes &other) const
    {
        return !(*this == other);
    }

    uint8_t bytes[count];
};


template<size_t count, size_t ...Index>
void DescribeIdBytes(
    std::ostream &outputStream,
    const IdBytes<count> &idBytes,
    std::index_sequence<Index...>)
{
    ((outputStream << ((Index == 0) ? "" : ", ") << idBytes.bytes[Index]), ...);
}


template<size_t count>
std::ostream & operator<<(
    std::ostream &outputStream,
    const IdBytes<count> &idBytes)
{
    DescribeIdBytes(
        outputStream,
        idBytes,
        std::make_index_sequence<count>{});

    return outputStream;
}

} // end namespace jive
