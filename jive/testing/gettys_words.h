/**
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright 2020 Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  */

#pragma once

#include <string_view>
#include <array>
#include <limits>
#include <random>
#include <string_view>
#include <algorithm>

// All the unique words used in the Gettysburg Address.
inline constexpr std::array<std::string_view, 138> gettysWords{
    "little",
    "dead",
    "god",
    "or",
    "our",
    "far",
    "might",
    "do",
    "four",
    "can",
    "larger",
    "live",
    "proposition",
    "engaged",
    "lives",
    "forth",
    "did",
    "we",
    "final",
    "new",
    "honored",
    "measure",
    "met",
    "the",
    "great",
    "earth",
    "their",
    "rather",
    "vain",
    "civil",
    "detract",
    "devotion",
    "fitting",
    "on",
    "consecrated",
    "birth",
    "perish",
    "consecrate",
    "in",
    "this",
    "fathers",
    "these",
    "field",
    "add",
    "dedicate",
    "who",
    "so",
    "nation",
    "have",
    "endure",
    "continent",
    "those",
    "task",
    "altogether",
    "nobly",
    "score",
    "it",
    "will",
    "long",
    "people",
    "is",
    "proper",
    "battle-field",
    "testing",
    "gave",
    "struggled",
    "be",
    "died",
    "resting",
    "brought",
    "seven",
    "under",
    "men",
    "resolve",
    "as",
    "created",
    "government",
    "liberty",
    "now",
    "dedicated",
    "hallow",
    "power",
    "for",
    "world",
    "but",
    "that",
    "highly",
    "above",
    "place",
    "ago",
    "cause",
    "unfinished",
    "say",
    "full",
    "are",
    "which",
    "portion",
    "from",
    "before",
    "brave",
    "conceived",
    "note",
    "take",
    "here",
    "years",
    "work",
    "fought",
    "freedom",
    "and",
    "they",
    "whether",
    "any",
    "remaining",
    "never",
    "thus",
    "what",
    "a",
    "equal",
    "come",
    "last",
    "war",
    "to",
    "shall",
    "living",
    "remember",
    "all",
    "poor",
    "by",
    "should",
    "sense",
    "nor",
    "forget",
    "ground",
    "us",
    "not",
    "of",
    "advanced",
    "increased"};


class RandomGettysWords
{
private:
        // Average word length in gettysWords is 4.27 characters, with
        // a maximum of 12 characters.
        // Pre-allocate the result string to probably avoid having to
        // re-allocate.
        // A space will be added between each word, so figure on 6 characters
        // per word.
        static constexpr auto averageWordLength = 6;

public:
    std::string MakeWords(size_t wordCount)
    {
        size_t estimated = averageWordLength * wordCount;
        std::string result;
        result.reserve(estimated);
        this->Make_(result, wordCount);
        return result;
    }

    std::string MakeLetters(size_t maxLetterCount)
    {
        size_t wordCount = maxLetterCount / averageWordLength;
        std::string result;
        result.reserve(maxLetterCount);
        this->Make_(result, wordCount);

        if (result.size() > maxLetterCount)
        {
            result.resize(maxLetterCount);
        }

        return result;
    }

    std::vector<std::string> MakeUniqueVector(size_t wordCount)
    {
        if (wordCount > gettysWords.size())
        {
            throw std::logic_error("Cannot provide so many unique words.");
        }

        std::vector<std::string> words;

        std::uniform_int_distribution<size_t> distribution(
            0,
            gettysWords.size() - 1);

        while (wordCount--)
        {
            auto word =
                std::string(gettysWords[distribution(this->generator_)]);

            if (std::find(words.begin(), words.end(), word) != words.end())
            {
                // Already added the word.
                continue;
            }

            words.emplace_back(word);
        }

        return words;
    }

    std::string MakeWord()
    {
        std::uniform_int_distribution<size_t> distribution(
            0,
            gettysWords.size() - 1);

        return std::string(gettysWords[distribution(this->generator_)]);
    }

    RandomGettysWords & Seed(size_t seed)
    {
        this->generator_.seed(seed);
        return *this;
    }

private:
    void Make_(std::string& result, size_t wordCount)
    {
        std::uniform_int_distribution<size_t> distribution(
            0,
            gettysWords.size() - 1);

        while (wordCount--)
        {
            result += gettysWords[distribution(this->generator_)];
            result += ' ';
        }
    }

    std::mt19937_64 generator_;
};
