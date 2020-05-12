/**
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright 2020 Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  */
#include <catch2/catch.hpp>

#include <iostream>
#include "jive/buffer.h"
#include "jive/power.h"


struct TrivialStruct
{
    int32_t a;
    float b;
    double c;
};

template<size_t align>
struct Align
{
    static constexpr auto value = align;
};

template<typename T, typename Align>
struct BufferTypes
{
    using type = T;
    static constexpr auto align = Align::value;
    static constexpr auto enable = (Align::value >= alignof(T));
};

using _8 = Align<8>;
using _16 = Align<16>;
using _32 = Align<32>;
using _64 = Align<64>;
using _128 = Align<128>;

#define MAKE_TYPE(name, type)               \
template<typename Align>                      \
struct name: BufferTypes<type, Align> {};   \

MAKE_TYPE(Int8, int8_t);
MAKE_TYPE(UInt8, uint8_t);
MAKE_TYPE(Int16, int16_t);
MAKE_TYPE(UInt16, uint16_t);
MAKE_TYPE(Int32, int32_t);
MAKE_TYPE(UInt32, uint32_t);
MAKE_TYPE(Int64, int64_t);
MAKE_TYPE(UInt64, uint64_t);
MAKE_TYPE(Float, float);
MAKE_TYPE(Double, double);
MAKE_TYPE(Trivial, TrivialStruct);


TEMPLATE_PRODUCT_TEST_CASE(
    "Create aligned Buffer.",
    "[buffer]",
    (Int8,
     UInt8,
     Int16,
     UInt16,
     Int32,
     UInt32,
     Int64,
     UInt64,
     Float,
     Double,
     Trivial),
    (_8, _16, _32, _64, _128))
{
    if constexpr(TestType::enable)
    {
        auto elementCount = GENERATE(
            take(
                20,
                random(size_t{1}, size_t{jive::Power<2, 20>()})));
        
        using Type = typename TestType::type;
        constexpr auto align = TestType::align;
        
        try
        {
            auto buffer = jive::Buffer<Type, align>(elementCount);

            REQUIRE(
                reinterpret_cast<uintptr_t>(buffer.Get()) % TestType::align == 0);

            REQUIRE(buffer.GetElementCount() == elementCount);
            REQUIRE(buffer.GetByteCount()
                == sizeof(typename TestType::type) * elementCount);
        }
        catch (std::bad_alloc &)
        {
            std::cout << "Failed to allocate " << elementCount << " buffer."
                << std::endl;
            throw;
        }
        
    }
}
