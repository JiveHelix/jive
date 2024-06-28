#pragma once


#include <atomic>
#include <stdexcept>
#include <cassert>


#ifndef NDEBUG
#include <iostream>
#endif


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
#ifndef NDEBUG
        if (this->flag_)
        {
            throw std::logic_error(
                "flag is already set. Did you intend to use CountFlag?");
        }
#endif
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

#ifndef NDEBUG

    CountFlag(const CountFlag &other)
        :
        count_{}
    {
        bool test = (other.count_ == 0);

        if (!test)
        {
            std::cerr
                << __FUNCTION__ << ":" << __LINE__
                << " Stern Warning: other.count_ is not zero!"
                << std::endl;
        }
    }

    CountFlag<T> & operator=(const CountFlag &other)
    {
        this->count_ = 0;

        // TODO: on Apple Clang 13.0.0 target x86_64-apple-darwin20.6.0
        // assert(other.count_ == 0) causes SIGILL EXC_I386_INVOP
        // assert(test) causes SIGILL EXC_I386_INVOP
        // Why is assert borked within this header?
        bool test = (other.count_ == 0);

        if (!test)
        {
            std::cerr
                << __FUNCTION__ << ":" << __LINE__
                << " Stern Warning: other.count_ is not zero!"
                << std::endl;
        }

        return *this;
    }

#else

    CountFlag(const CountFlag &)
        :
        count_{}
    {

    }

    CountFlag<T> & operator=(const CountFlag &)
    {
        this->count_ = 0;

        return *this;
    }

#endif

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

    T GetCount() const
    {
        return this->flag_.GetCount();
    }

private:
    CountFlag<T> &flag_;
};


} // end namespace jive
