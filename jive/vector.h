#pragma once


#include <cstdint>
#include <vector>
#include <limits>


namespace jive
{


template<typename T>
auto IndexIterator(std::vector<T> &vector, size_t index)
{
    assert(index <= std::numeric_limits<std::ptrdiff_t>::max());
    assert(index < vector.size());

    return std::begin(vector) + static_cast<std::ptrdiff_t>(index);
}

template<typename T>
auto SafeEraseIterator(std::vector<T> &vector, size_t index)
{
    assert(index <= std::numeric_limits<std::ptrdiff_t>::max());

    if (index >= vector.size())
    {
        throw std::out_of_range("element does not exist");
    }

    return std::begin(vector) + static_cast<std::ptrdiff_t>(index);
}


template<typename T>
auto SafeInsertIterator(std::vector<T> &vector, size_t index)
{
    assert(index <= std::numeric_limits<std::ptrdiff_t>::max());

    if (index > vector.size())
    {
        throw std::out_of_range("element does not exist");
    }

    return std::begin(vector) + static_cast<std::ptrdiff_t>(index);
}


template<typename T>
void Erase(std::vector<T> &vector, size_t index)
{
    vector.erase(IndexIterator(vector, index));
}


template<typename T>
void SafeErase(std::vector<T> &vector, size_t index)
{
    vector.erase(SafeEraseIterator(vector, index));
}


} // end namespace jive
