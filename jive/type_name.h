/**
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  */

#pragma once

#include <string>
#include <typeinfo>
#include <cstdlib>
#include <memory>
#include <cxxabi.h>

namespace jive
{

template <class T>
std::string GetTypeName()
{
    // Compiler warns about uninitialized value.
    int status = -4;

    auto name = typeid(T).name();

    auto result = std::unique_ptr<char, decltype(std::free) *>{
        abi::__cxa_demangle(name, nullptr, nullptr, &status),
        std::free};

    return (status == 0) ? result.get() : name;
}

} // end namespace jive
