#pragma once


#include <atomic>


namespace jive
{


template<typename Flag = bool>
class ScopeFlag
{
public:
    ScopeFlag(Flag &flag)
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
    Flag &flag_;
};


template<typename T>
class CountFlag
{
public:
    CountFlag()
        :
        count_{}
    {

    }

    operator bool () const
    {
        return this->count_ != 0;
    }

    CountFlag & operator++()
    {
        ++this->count_;
        return *this;
    }

    CountFlag & operator--()
    {
        if (this->count_ == 0)
        {
            throw std::logic_error("Count is 0");
        }

        --this->count_;
        return *this;
    }

    T GetCount() const
    {
        return this->count_;
    }

private:
    std::atomic<T> count_;
};


template<typename T>
class ScopedCountFlag
{
public:
    ScopedCountFlag(CountFlag<T> &flag)
        :
        flag_(flag)
    {
        ++this->flag_;
    }

    ~ScopedCountFlag()
    {
        --this->flag_;
    }

    ScopedCountFlag(const ScopedCountFlag &) = delete;
    ScopedCountFlag(ScopedCountFlag &&) = delete;
    ScopedCountFlag & operator=(const ScopedCountFlag &) = delete;
    ScopedCountFlag & operator=(ScopedCountFlag &&) = delete;

private:
    CountFlag<T> &flag_;
};


} // end namespace jive
