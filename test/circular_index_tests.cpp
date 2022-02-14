#include <catch2/catch.hpp>

#include <iostream>
#include "jive/circular_index.h"

/*
 * In compiler
 *    Apple clang version 13.0.0 (clang-1300.0.29.3)
 * and probably in others:
 * 
 * Before CircularIndex had non-member operator+ and operator- overloads,
 * the class method GetSize() implicitly converted both operands to size_t
 * before applying the substraction, possibly resulting in very large unsigned
 * (small negative) results.
 *
 * The compiler chose the member operator- of the CircularIndex in
 * all other contexts.
 *
 * The problem has been resolved by adding the non-member operator+ and
 * operator- overloads.
 *
 */

template<size_t N>
class TestBuffer
{
public:
    void Read(size_t count)
    {
        this->readIndex += jive::CircularIndex<N>(count);
    }

    void Write(size_t count)
    {
        this->writeIndex += jive::CircularIndex<N>(count);
    }

    size_t GetSize() const
    {
        return this->writeIndex - this->readIndex;
    }

    jive::CircularIndex<N> readIndex;
    jive::CircularIndex<N> writeIndex;
};


template<size_t N>
size_t GetSize(jive::CircularIndex<N> left, jive::CircularIndex<N> right)
{
    return left - right;
}



TEST_CASE("CircularIndex wraps", "[circular_index]")
{
    using Index = jive::CircularIndex<8>;
    Index index(7);;
    ++index;

    REQUIRE(0 == index);
    index += Index(12);
    REQUIRE(4 == index);
}


TEST_CASE("CircularIndex subtracts", "[circular_index]")
{
    TestBuffer<1024> testBuffer;
    testBuffer.Write(1024);
    testBuffer.Read(1016);

    // Create my own size
    // The compiler correctly finds CircularIndex::operator-
    size_t mySize = testBuffer.writeIndex - testBuffer.readIndex;

    // Let a non-member function compute the difference.
    // The compiler correctly finds CircularIndex::operator-
    size_t bufferSize = GetSize(testBuffer.writeIndex, testBuffer.readIndex);
    REQUIRE(bufferSize == mySize);

    // Let the member function GetSize compute the difference.

    // In jive/1.0.4, the compiler converts both to size_t, ignoring the member
    // operator-, and the test fails.
    //
    // in jive/1.0.5, the compiler uses the non-member operator- overload.
    bufferSize = testBuffer.GetSize();
    REQUIRE(bufferSize == mySize);
    REQUIRE(bufferSize == 8);

    testBuffer.Write(8);

    REQUIRE(testBuffer.GetSize() == 16);
}
