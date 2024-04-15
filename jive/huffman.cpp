#include "jive/huffman.h"


namespace huffman
{


size_t FrequencyNode::nextCreationIndex = 0;


std::ostream & operator<<(std::ostream &output, const FrequencyNode &node)
{
    output << "FrequencyNode(" << node.frequency;

    if (node.value)
    {
        output << ", '" << *node.value << "'";
    }

    return output << ")";
}


std::ostream & operator<<(std::ostream &output, const FrequencyNodePointer &node)
{
    return output << *node << std::endl;
}


bool operator<(
    const FrequencyNodePointer &left,
    const FrequencyNodePointer &right)
{
    return (*left < *right);
}


void Traverse(
    std::map<char, Code> &codeByLetter,
    Code &code,
    const Node *node)
{
    if (node->value)
    {
        codeByLetter[*node->value] = code;

        return;
    }

    if (node->left)
    {
        Traverse(codeByLetter, code.Push(false), node->left.get());
        code.Pop();
    }

    if (node->right)
    {
        Traverse(codeByLetter, code.Push(true), node->right.get());
        code.Pop();
    }
}


void WriteNode(std::ostream &output, char value, const Code &code)
{
    const auto &turns = code.GetTurns();

    if (turns.size() > 255)
    {
        throw std::runtime_error("Cannot encode more than 255 bits");
    }

    auto bitCount = static_cast<uint8_t>(turns.size());
    jive::io::Write(output, value);
    jive::io::Write(output, bitCount);

    uint8_t buffer{};
    int bitIndex = 7;

    for (size_t i = 0; i < turns.size(); ++i)
    {
        buffer = static_cast<uint8_t>(
            buffer
            | (turns[i] << bitIndex));

        --bitIndex;

        if (bitIndex < 0)
        {
            jive::io::Write(output, buffer);
            bitIndex = 7;
            buffer = 0;
        }
    }

    if (bitIndex != 7)
    {
        // There is one more byte to write.
        jive::io::Write(output, buffer);
    }
}


void ReadNode(std::istream &input, Node *root)
{
    auto value = jive::io::Read<char>(input);
    auto bitCount = jive::io::Read<uint8_t>(input);

    uint8_t buffer{};
    int bitIndex = 7;

    Node *node = root;

    for (uint8_t i = bitCount; i > 0; --i)
    {
        if (bitIndex == 7)
        {
            buffer = jive::io::Read<uint8_t>(input);
        }

        auto mask = static_cast<uint8_t>(1 << bitIndex);

        if (buffer & mask)
        {
            // True is a right turn.
            if (!node->right)
            {
                node->right = std::make_shared<Node>();
            }

            node = node->right.get();
        }
        else
        {
            // Left turn
            if (!node->left)
            {
                node->left = std::make_shared<Node>();
            }

            node = node->left.get();
        }

        --bitIndex;

        if (bitIndex < 0)
        {
            bitIndex = 7;
        }
    }

    node->value = value;
}


void TraverseWrite(
    std::ostream &output,
    std::map<char, Code> &codeByLetter,
    Code &code,
    Node *node)
{
    if (node->value)
    {
        codeByLetter[*node->value] = code;
        WriteNode(output, *node->value, code);

        return;
    }

    if (node->left)
    {
        TraverseWrite(output, codeByLetter, code.Push(false), node->left.get());
        code.Pop();
    }

    if (node->right)
    {
        TraverseWrite(output, codeByLetter, code.Push(true), node->right.get());
        code.Pop();
    }
}


NodeTree BuildTree(const char *data, size_t count)
{
    std::map<char, FrequencyNode> nodesByLetter;

    for (size_t i = 0; i < count; ++i)
    {
        nodesByLetter[data[i]].frequency++;
    }

    std::deque<std::shared_ptr<FrequencyNode>> sortedNodes;

    for (auto & [letter, node]: nodesByLetter)
    {
        node.value = letter;
        sortedNodes.push_back(std::make_shared<FrequencyNode>(node));
    }

    std::sort(std::begin(sortedNodes), std::end(sortedNodes));

    while (sortedNodes.size() > 1)
    {
        sortedNodes.push_back(
            std::make_shared<FrequencyNode>(
                sortedNodes[0],
                sortedNodes[1]));

        sortedNodes.pop_front();

        if (sortedNodes.size() > 1)
        {
            sortedNodes.pop_front();
        }

        std::sort(std::begin(sortedNodes), std::end(sortedNodes));
    }

    return {sortedNodes[0], nodesByLetter.size()};
}


size_t Compress(std::ostream &output, const char *data, size_t byteCount)
{
    auto start = output.tellp();
    auto nodeTree = BuildTree(data, byteCount);
    OutputBitstream outputBitstream(output, byteCount, nodeTree);

    while (byteCount--)
    {
        outputBitstream.Write(*(data++));
    }

    outputBitstream.Flush();

    return static_cast<size_t>(output.tellp() - start);
}


} // end namespace huffman
