/**
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright 2020 Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  */

#pragma once

#include <functional>
#include <cmath>
#include <algorithm>

#include "jive/circular_index.h"


namespace jive
{


/**
 ** Uses memoization to store running sum to speed the calculation of the
 ** average value.
 **
 ** @tparam T The type of the stored elements
 ** @tparam elementCount
 **/
template<typename T, size_t elementCount>
class AveragingWindow
{
public:
    AveragingWindow()
        :
        sum_(0),
        index_(),
        elements_{},
        isFull_(false)
    {

    }

    AveragingWindow(T initialValue)
        :
        sum_(initialValue * elementCount),
        index_(),
        elements_{},
        isFull_(true)
    {
        std::fill_n(&this->elements_[0], elementCount, initialValue);
    }

    void AddElement(T element)
    {
        // We are about to replace an existing element
        // Remove the existing element from the sum_
        this->sum_ -= this->elements_[this->index_];

        // Add the new element
        this->elements_[this->index_] = element;

        // Update the sum_
        this->sum_ += element;
        ++this->index_;

        this->CheckIsFull_();
    }

    bool GetIsFull() const { return this->isFull_; }

    T GetAverage() const
    {
        return this->sum_ / elementCount;
    }

    double GetAverageAsDouble() const
    {
        return static_cast<double>(this->sum_) / elementCount;
    }

    double GetVariance(double average) const
    {
        double sumOfSquaredDifferences = 0;

        for (T element: this->elements_)
        {
            double difference = element - average;
            sumOfSquaredDifferences += (difference * difference);
        }

        return sumOfSquaredDifferences / elementCount;
    }

    double GetVariance() const
    {
        return this->GetVariance(this->GetAverage());
    }

    double GetStandardDeviation(double average) const
    {
        return std::sqrt(this->GetVariance(average));
    }

    double GetStandardDeviation() const
    {
        return this->GetStandardDeviation(this->GetAverageAsDouble());
    }

    void Reset()
    {
        this->index_.Reset();
        std::fill_n(&this->elements_[0], elementCount, 0);
        this->sum_ = 0;
        this->isFull_ = false;
    }

    T GetMinimum() const
    {
        const T *begin = &this->elements_[0];
        const T *end = begin + elementCount;
        return *std::min_element(begin, end);
    }

    T GetMaximum() const
    {
        const T *begin = &this->elements_[0];
        const T *end = begin + elementCount;
        return *std::max_element(begin, end);
    }

private:
    void CheckIsFull_()
    {
        if (this->isFull_)
        {
            return;
        }

        if (static_cast<size_t>(this->index_) == 0)
        {
            // Advancing the CircularIndex rolled back to zero, so we know that
            // we have assigned all 2^N elements.
            this->isFull_ = true;
        }
    }

protected:
    T sum_;

private:
    CircularIndex<elementCount> index_;
    T elements_[elementCount];
    bool isFull_;
};


} // end namespace jive
