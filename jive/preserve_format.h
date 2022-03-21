/**
  * @file preserve_format.h
  * 
  * @brief Maintain stream format in current scope.
  * 
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 28 Mar 2022
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
**/

#pragma once

#include <ios>


namespace jive
{

template<typename CharT>
class PreserveFormat
{
public:
    using Stream = std::basic_ios<CharT>;

    PreserveFormat(Stream &stream)
        :
        stream_(stream),
        preserved_(nullptr)
    {
        this->preserved_.copyfmt(stream);
    }

    ~PreserveFormat()
    {
        this->stream_.copyfmt(this->preserved_);
    }

private:
    Stream &stream_;
    Stream preserved_;
};


} // end namespace jive
