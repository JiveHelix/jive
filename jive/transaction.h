/**
  * @file transaction.h
  *
  * @brief Performs a series of function calls, and stores an undo function to
  * roll back any operations in the event of failure.
  *
  * @author Jive Helix (jivehelix@gmail.com)
  * @date 17 May 2018
  * @copyright Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  */

#pragma once

#include "jive/detail/transaction_detail.h"

namespace jive
{

class Transaction
{
public:
    Transaction()
        :
        operationStack_()
    {

    }

    Transaction(const Transaction &) = delete;
    Transaction(Transaction &&) = delete;
    Transaction & operator=(const Transaction &) = delete;
    Transaction & operator=(Transaction &&) = delete;

    ~Transaction()
    {
        // Destroy operations from the stack in reverse order.
        while (this->operationStack_.size())
        {
            this->operationStack_.pop_back();
        }
    }

    void operator()(
        detail::Operation::FunctionT doOperation,
        detail::Operation::FunctionT undoOperation)
    {
        this->operationStack_.push_back(
            detail::Operation(doOperation, undoOperation));
    }

    void Commit()
    {
        for (auto &it: this->operationStack_)
        {
            it.Commit();
        }
    }

private:
    std::vector<detail::Operation> operationStack_;
};

} // end namespace jive
