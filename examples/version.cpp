#include <sstream>
#include <iostream>
#include <jive/version.h>


int main()
{
    using namespace jive;

    Version<int8_t> signed8(-2, 42, 0);
    Version<uint8_t> unsigned8(2, 143, 9);
    Version<int16_t> signed16(-22000, 19, -1000);
    Version<uint16_t> unsigned16(4338, 65001, 1000);

    std::cout << signed8.ToString() << std::endl;
    std::cout << unsigned8.ToString() << std::endl;
    std::cout << signed16.ToString() << std::endl;
    std::cout << unsigned16.ToString() << std::endl;

    std::stringstream test;

    signed8.Write(test);
    unsigned8.Write(test);
    signed16.Write(test);
    unsigned16.Write(test);

    auto recoveredSigned8 = Version<int8_t>::Read(test);
    auto recoveredUnsigned8 = Version<uint8_t>::Read(test);

    auto recoveredSigned16 = Version<int16_t>::Read(test);
    auto recoveredUnsigned16 = Version<uint16_t>::Read(test);

    std::cout << recoveredSigned8.ToString() << std::endl;
    std::cout << recoveredUnsigned8.ToString() << std::endl;
    std::cout << recoveredSigned16.ToString() << std::endl;
    std::cout << recoveredUnsigned16.ToString() << std::endl;

    return 0;
}
