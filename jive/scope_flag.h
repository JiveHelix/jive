#pragma once


namespace jive
{


class ScopeFlag
{
public:
    ScopeFlag(bool &flag)
        :
        flag_(flag)
    {
        this->flag_ = true;
    }

    ~ScopeFlag()
    {
        this->flag_ = false;
    }

    ScopeFlag(const ScopeFlag &) = delete;
    ScopeFlag(ScopeFlag &&) = delete;
    ScopeFlag & operator=(const ScopeFlag &) = delete;
    ScopeFlag & operator=(ScopeFlag &&) = delete;

private:
    bool &flag_;
};


} // end namespace jive
