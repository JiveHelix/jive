/**
  * @file eigen.h
  * 
  * @brief A few convenience functions for working with Eigen.
  * 
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 19 May 2020
  * Licensed under the MIT license. See LICENSE file.
**/
#pragma once

#include <type_traits>
#include "Eigen/Dense"
#include "jive/future.h"

namespace jive
{

/** Eigen 3.3.7 does not support brace initialization, and comma initialization
 ** is unsafe because it does not fail until runtime when Eigen discovers an
 ** incorrect number of arguments.
 **
 ** This helper allows comma initialization to be used as a one-liner, and
 ** provides a compile time check on the argument count.
 **/
template<int columns, int rows, typename T, typename ...Values>
Eigen::Matrix<std::remove_cvref_t<T>, columns, rows>
EigenMatrix(T &&first, Values &&...values)
{
    static_assert(sizeof...(Values) + 1 == columns * rows);
    Eigen::Matrix<std::remove_cvref_t<T>, columns, rows> result;
    ((result << std::forward<T>(first)), ..., std::forward<Values>(values));
    return result;
}


/**
 ** Numpy, Matlab, and others support fancy indexing, where
 **     myArray[myArray > 2.0] = 2.0
 ** will replace any values in myArray that are greater than 2 with 2.
 **
 ** To achieve the same in Eigen requires
 **     (myArray > 2.0).select(2.0, myArray);
 ** which feels awkward to use.
 **
 ** This adapter doesn't match the common syntax exactly, but expresses the
 ** intent more clearly:
 **     Select(myArray) > 2.0 = 2.0;
 **
 ** All six comparison operators are supported. Perfect forwarding is used for
 ** zero overhead.
 **/
template<typename MatrixType>
class Select
{
public:
    Select(MatrixType &matrix): matrix_(matrix) {}

    template<typename Value>
    auto operator>(Value &&value)
    {
        return this->MakeAssigner(
            this->matrix_.array() > std::forward<Value>(value));
    }

    template<typename Value>
    auto operator<(Value &&value)
    {
        return this->MakeAssigner(
            this->matrix_.array() < std::forward<Value>(value));
    }

    template<typename Value>
    auto operator<=(Value &&value)
    {
        return this->MakeAssigner(
            this->matrix_.array() <= std::forward<Value>(value));
    }

    template<typename Value>
    auto operator>=(Value &&value)
    {
        return this->MakeAssigner(
            this->matrix_.array() >= std::forward<Value>(value));
    }

    template<typename Value>
    auto operator==(Value &&value)
    {
        return this->MakeAssigner(
            this->matrix_.array() == std::forward<Value>(value));
    }

    template<typename Value>
    auto operator!=(Value &&value)
    {
        return this->MakeAssigner(
            this->matrix_.array() != std::forward<Value>(value));
    }

private:
    template<typename Q>
    class Assigner
    {
    public:
        // Add rvalue referent to Q to force the forwarding reference.
        // Constructors do not have template arguments, so we cannot use Q &&q
        // like we would in a templated function.
        Assigner(const MatrixType &matrix, std::add_rvalue_reference_t<Q> q)
            :
            matrix_(matrix),
            q_(std::forward<Q>(q))
        {

        }

        template<typename Value>
        MatrixType operator=(Value && value)
        {
            return this->q_.select(std::forward<Value>(value), this->matrix_);
        }

        const MatrixType &matrix_;
        Q q_;
    };

    template<typename U>
    Assigner<U> MakeAssigner(U &&u)
    {
        return Assigner<U>(this->matrix_, std::forward<U>(u));
    }

private:
    const MatrixType &matrix_;
};


} // end namespace jive
