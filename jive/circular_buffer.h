/**
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  */
#pragma once

#include "jive/circular_index.h"

namespace jive
{

template<typename T, size_t N>
class CircularBuffer
{
public:
    CircularBuffer()
        :
        isFull_(false),
        index_()
    {

    }

    void Reset()
    {
        this->isFull_ = false;
        this->index_.Reset();
    }
    
    bool IsEmpty()
    {
        return (!this->isFull_ && this->index_ == 0);
    }

    void AddValue(T value)
    {
        this->elements_[this->index_++] = value;
        
        if (!this->isFull_)
        {
            if (this->index_ == 0)
            {
                this->isFull_ = true;
            }
        }
    }

    T FrontElement()
    {
        return this->elements_[this->BeginIndex()];
    }

    CircularIndex<N> BeginIndex()
    {
        if (this->isFull_)
        {
            return this->index_;
        }
        else
        {
            // Partial array. The zeroth element is the first
            return CircularIndex<N>();
        }
    }
    
    CircularIndex<N> EndIndex()
    {
        return this->index_;
    }

    T BackElement()
    {
        return this->elements_[--this->EndIndex()];
    }

    T Sum()
    {
        auto index = this->BeginIndex();
        auto stop = this->EndIndex();

        T sum{};
        
        if (!this->isFull_)
        {
            if (0 == this->index_)
            {
                // there is no data yet.
                return sum;
            }
        }

        do
        {
            sum += this->elements_[index++];    
        }
        while (index != stop);

        return sum;
    }

    T Average()
    {
        if (this->isFull_)
        {
            return this->Sum() / static_cast<T>(N);
        }
        else
        {
            return this->Sum()
                / static_cast<T>(static_cast<size_t>(this->index_));
        }
    }

    void PrintElements()
    {
        for (size_t i = 0; i < N; i++)
        {
            std::cout << this->elements_[i] << " ";
        }

        std::cout << std::endl;
    }

private:
    bool isFull_;
    CircularIndex<N> index_;
    T elements_[N];
};

} // end namespace jive
