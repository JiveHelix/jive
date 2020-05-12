/**
  * @file create_exception.h
  * 
  * @brief A macro to shorten the creation of new exceptions.
  * 
  * @author Jive Helix (jivehelix@gmail.com)
  * Licensed under the MIT license. See LICENSE file.
  */


#pragma once

#include <stdexcept>
#include <type_traits>


/*
 * A macro shortcut for creating new exceptions.
 *
 * @param exceptionName A name for the new exception.
 * @param parentClass The parent of the new exception.
 *
 * The parentClass must be derived from std::exception, but it cannot be
 * std::exception, because std::exception does not have a ctor with arguments.
 */
#define CREATE_EXCEPTION(exceptionName, parentClass)                    \
    static_assert(                                                      \
        std::is_base_of<std::exception, parentClass>::value,            \
        "std::exception must be base of parentClass");                  \
                                                                        \
    static_assert(                                                      \
        !std::is_same<std::exception, parentClass>::value,              \
        "parentClass must not be std::exception");                      \
                                                                        \
    class exceptionName : public parentClass                            \
    {                                                                   \
    public:                                                             \
        exceptionName(const std::string &what) : parentClass(what) {}   \
    }
