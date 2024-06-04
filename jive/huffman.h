#pragma once

#include <deque>
#include <vector>
#include <map>
#include <ostream>
#include <istream>
#include <cassert>
#include <memory>
#include <optional>

#include "jive/binary_io.h"


namespace huffman
{


struct Node
{
    std::optional<char> value;
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;

    Node()
        :
        value(),
        left(),
        right()
    {

    }

    Node(
        const std::shared_ptr<Node> &left_,
        const std::shared_ptr<Node> &right_)
        :
        value(),
        left(left_),
        right(right_)
    {

    }
};


struct FrequencyNode: public Node
{
    static size_t nextCreationIndex;

    size_t creationIndex;
    size_t frequency;

    FrequencyNode()
        :
        Node(),
        creationIndex(nextCreationIndex++),
        frequency()
    {

    }

    FrequencyNode(
        const std::shared_ptr<FrequencyNode> &left_,
        const std::shared_ptr<FrequencyNode> &right_)
        :
        Node(left_, right_),
        creationIndex(nextCreationIndex++),
        frequency(left_->frequency + right_->frequency)
    {

    }

    bool operator<(const FrequencyNode &other) const
    {
        if (this->frequency == other.frequency)
        {
            if (this->value)
            {
                if (other.value)
                {
                    // Sort by value
                    return *this->value < *other.value;
                }
                else
                {
                    // Leaf nodes always come first in a tie.
                    return true;
                }
            }

            assert(!this->value);

            if (other.value)
            {
                // Leaf nodes always come first in a tie.
                return false;
            }

            // Neither node is a leaf node.
            // Settle the tie with creation order.
            return this->creationIndex < other.creationIndex;
        }

        return this->frequency < other.frequency;
    }
};


using FrequencyNodePointer = std::shared_ptr<FrequencyNode>;


std::ostream & operator<<(std::ostream &output, const FrequencyNode &node);


std::ostream & operator<<(
    std::ostream &output,
    const FrequencyNodePointer &node);


bool operator<(
    const FrequencyNodePointer &left,
    const FrequencyNodePointer &right);


class Code
{
public:
    Code()
        :
        turns_()
    {

    }

    Code & Push(bool isRight)
    {
        this->turns_.push_back(isRight);
        return *this;
    }

    void Pop()
    {
        this->turns_.pop_back();
    }

    template<typename T>
    std::pair<size_t, T> GetCode() const
    {
        T result{};

        if (this->turns_.size() > sizeof(T) * 8)
        {
            throw std::runtime_error("Too many turns to encode");
        }

        auto it = std::begin(this->turns_);

        while (it != std::end(this->turns_))
        {
            result = (result << 1) | T(*it++);
        }

        return std::make_pair(this->turns_.size(), result);
    }

    void Describe(std::ostream &output) const
    {
        auto code = this->GetCode<uint64_t>();
        auto mask = 1ULL << (code.first - 1);

        for (size_t i = 0; i < code.first; ++i)
        {
            output << static_cast<bool>(code.second & (mask >> i));
        }
    }

    const std::vector<bool> & GetTurns() const
    {
        return this->turns_;
    }

private:
    std::vector<bool> turns_;
};


void Traverse(
    std::map<char, Code> &codeByLetter,
    Code &code,
    const Node *node);


void WriteNode(std::ostream &output, char value, const Code &code);


void ReadNode(std::istream &input, Node *root);


void TraverseWrite(
    std::ostream &output,
    std::map<char, Code> &codeByLetter,
    Code &code,
    Node *node);


struct NodeTree
{
    std::shared_ptr<Node> root;
    size_t count;
};


NodeTree BuildTree(std::istream &input, size_t count);


class OutputBitstream
{
public:
    OutputBitstream(
        std::ostream &output,
        size_t expandedSize,
        const NodeTree &nodeTree)
        :
        output_(output),
        buffer_{},
        bitIndex_(7),
        codeByLetter_{}
    {
        if (expandedSize > 65535)
        {
            throw std::runtime_error("Exceeds current compression limit.");
        }

        jive::io::Write(this->output_, static_cast<uint16_t>(expandedSize));
        jive::io::Write(this->output_, static_cast<uint8_t>(nodeTree.count));

        Code code{};

        // Write the node tree to the output while building the code map.
        TraverseWrite(
            this->output_,
            this->codeByLetter_,
            code,
            nodeTree.root.get());

#ifdef VERBOSE
        std::cout << "size of header: " << dataStart << std::endl;

        for (auto letterCode: this->codeByLetter_)
        {
            std::cout << letterCode.first << ": ";
            letterCode.second.Describe(std::cout);
            std::cout << std::endl;
        }
#endif
    }

    void Write(char value)
    {
        this->Write(this->codeByLetter_.at(value));
    }

    void Write(const Code &code)
    {
        const auto &turns = code.GetTurns();

        for (size_t i = 0; i < turns.size(); ++i)
        {
            this->buffer_ = static_cast<uint8_t>(
                this->buffer_
                | (turns[i] << this->bitIndex_));

            --this->bitIndex_;

            if (this->bitIndex_ == -1)
            {
                this->DoFlush_();
            }
        }
    }

    void Flush()
    {
        if (this->bitIndex_ < 7)
        {
            this->DoFlush_();
        }
    }

private:
    void DoFlush_()
    {
        jive::io::Write(this->output_, this->buffer_);
        this->bitIndex_ = 7;
        this->buffer_ = 0;
    }

    std::ostream &output_;
    uint8_t buffer_;
    int bitIndex_;
    std::map<char, Code> codeByLetter_;
};


class InputBitstream
{
public:
    InputBitstream(std::istream &input)
        :
        input_(input),
        root_{},
        buffer_{},
        bitIndex_(7),
        expandedSize_(jive::io::Read<uint16_t>(input)),
        expandedCount_(0)
    {
        auto recoveredSymbols = jive::io::Read<uint8_t>(input);

        while (recoveredSymbols--)
        {
#ifdef VERBOSE
            ssize_t startPosition = input.tellg();
#endif
            ReadNode(input, &this->root_);
#ifdef VERBOSE
            ssize_t endPosition = input.tellg();
            ssize_t thisSymbol = endPosition - startPosition;

            std::cout << "  consumed: " << thisSymbol << " bytes. "
                << endPosition - 3
                << " of " << dataStart - 3 << " bytes" << std::endl;
#endif
        }
    }

    size_t GetExpandedSize() const
    {
        return this->expandedSize_;
    }

    char ExpandValue()
    {
        if (this->expandedCount_ == this->expandedSize_)
        {
            throw std::logic_error("No more data to expand.");
        }

        const Node *node = &this->root_;

        while (!node->value)
        {
            if (this->bitIndex_ == 7)
            {
                this->buffer_ = jive::io::Read<uint8_t>(this->input_);
            }

            auto mask = static_cast<uint8_t>(1 << this->bitIndex_);

            if (this->buffer_ & mask)
            {
                // True is a right turn.
                node = node->right.get();
            }
            else
            {
                node = node->left.get();
            }

            --this->bitIndex_;

            if (this->bitIndex_ < 0)
            {
                this->bitIndex_ = 7;
            }
        }

        ++this->expandedCount_;

        return *node->value;
    }

    size_t GetCount() const
    {
        return this->expandedSize_ - this->expandedCount_;
    }

private:
    std::istream &input_;
    Node root_;
    uint8_t buffer_;
    int bitIndex_;
    size_t expandedSize_;
    size_t expandedCount_;
};


class Expander
{
public:
    Expander(std::istream &input)
        :
        inputBitstream_(input)
    {

    }

    size_t GetExpandedSize() const
    {
        return this->inputBitstream_.GetExpandedSize();
    }

    void Expand(std::ostream &output, size_t byteCount)
    {
        if (byteCount > this->inputBitstream_.GetExpandedSize())
        {
            throw std::out_of_range("byteCount exceeds availabe data");
        }

        while (byteCount--)
        {
            output.put(this->inputBitstream_.ExpandValue());
        }
    }

    void Expand(std::ostream &output)
    {
        size_t byteCount = this->inputBitstream_.GetExpandedSize();
        this->Expand(output, byteCount);
    }

private:
    InputBitstream inputBitstream_;
};


size_t Compress(
    std::ostream &output,
    std::istream &input,
    size_t byteCount);


} // end namespace huffman
