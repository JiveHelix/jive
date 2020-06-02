/**
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright 2011-2020 Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  */

#pragma once

#include <functional>

namespace jive
{

namespace detail
{

class Operation
{
public:
    using FunctionT = std::function<void()>;

public:
    Operation(FunctionT doOperation, FunctionT undoOperation)
        :
        undoOperation_([]{})
    {
        doOperation();

        // Now that the operation has completed, we can store the undo
        // operation for later cleanup, if necessary.
        this->undoOperation_ = undoOperation;
    }

    Operation(const Operation &) = delete;

    Operation(Operation &&other)
        :
        undoOperation_(other.undoOperation_)
    {
        // Change other's undoOperation_ to a no-op before it is destroyed.
        other.undoOperation_ = []{};
    }

    Operation & operator=(const Operation &) = delete;
    Operation & operator=(Operation &&) = delete;

    ~Operation()
    {
        this->undoOperation_();
    }

    void Commit()
    {
        // Replace the undo operation with a no-op.
        this->undoOperation_ = []{};
    }

private:
    FunctionT undoOperation_;
};

} // end namespace detail

} // end namespace jive
