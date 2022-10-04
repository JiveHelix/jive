/**
  * @file cleanup.h
  *
  * @brief Calls a cleanup function when Cleanup is destroyed.
  *
  * @author Jive Helix jivehelix@gmail.com)
  * @date 02 Mar 2018
  * @copyright Jive Helix
  *
  * Licensed under the MIT license. See LICENSE file.
  */

#pragma once

#include <functional>

namespace jive
{

class Cleanup
{
public:
    Cleanup(std::function<void()> cleanupFunction)
        :
        cleanupFunction_(cleanupFunction)
    {

    }

    ~Cleanup()
    {
        this->cleanupFunction_();
    }

private:
    std::function<void()> cleanupFunction_;
};

} // end namespace jive
