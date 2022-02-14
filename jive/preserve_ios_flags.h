#pragma once

#include <ios>


namespace jive
{


class PreserveIosFlags
{
public:
    PreserveIosFlags(std::ios_base &stream)
        :
        stream_(stream),
        flags_(stream.flags())
    {

    }

    ~PreserveIosFlags()
    {
        this->stream_.flags(this->flags_); 
    }

private:
    std::ios_base &stream_;
    std::ios_base::fmtflags flags_;
};


} // end namespace jive
