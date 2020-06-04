/**
  * @file format_paragraph.cpp
  *
  * @brief Converts a string into a vector of strings that fit within a line.
  *
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright 2011-2018 Jive Helix
  *
  * Licensed under the MIT license. See LICENSE file.
  */

#include "jive/format_paragraph.h"
#include "jive/strings.h"

#include <memory>
#include <deque>

namespace jive
{

class SingleLine
{
public:
    SingleLine(): totalWordLength_(0) {}

    size_t PredictLength(const std::string &newWord)
    {
        return this->GetLineLength() + newWord.size() + 1;
    }

    void AddWord(const std::string &newWord)
    {
        this->lineVector_.push_back(newWord);
        this->totalWordLength_ += newWord.size();
    }

    size_t GetLineLength()
    {
        // Add width occupied by spaces
        return this->totalWordLength_ + this->lineVector_.size() - 1;
    }

    std::string GetLine()
    {
        return strings::Join(
            this->lineVector_.begin(),
            this->lineVector_.end(),
            " ");
    }

    void Reset()
    {
        this->lineVector_.clear();
        this->totalWordLength_ = 0;
    }

private:
    std::vector<std::string> lineVector_;
    size_t totalWordLength_;
};


std::vector<std::string> FormatParagraph(
    const std::string &paragraph,
    size_t indentSpacesCount,
    size_t maxLineSpacesCount)
{
    SingleLine currentLine;
    auto words = strings::Split(paragraph, ' ');
    std::deque<std::string> wordDeque(words.begin(), words.end());
    std::vector<std::string> lines;

    if (indentSpacesCount > maxLineSpacesCount)
    {
        throw std::logic_error("Requested more indentation than width.");
    }

    size_t permittedLineLength = maxLineSpacesCount - indentSpacesCount;

    while (!wordDeque.empty())
    {
        bool nextWordHasNewline = ('\n' == wordDeque.front().back());
        if (currentLine.PredictLength(wordDeque.front()) > permittedLineLength)
        {
            // The next word would exceed the allowable line length.
            lines.push_back(
                std::string(indentSpacesCount, ' ') + currentLine.GetLine());

            currentLine.Reset();
            continue;
        }

        currentLine.AddWord(wordDeque.front());
        wordDeque.pop_front();

        if (nextWordHasNewline)
        {
            // Consider this a paragraph break
            lines.push_back(
                std::string(indentSpacesCount, ' ') + currentLine.GetLine());

            currentLine.Reset();
        }
    }

    if (currentLine.GetLineLength() > 0)
    {
        lines.push_back(currentLine.GetLine());
    }

    return lines;
}


} // end namespace jive
