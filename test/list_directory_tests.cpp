/**
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright 2020 Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  */
#include <catch2/catch.hpp>

#include <algorithm>

#include "jive/list_directory.h"
#include "utility/directory_creator.h"
#include "utility/gettys_words.h"


TEST_CASE(
    "ListDirectory returns a list of every file in a directory",
    "[list_directory]")
{
    auto fileCount = GENERATE(take(10, random(size_t{0}, size_t{30})));
    RandomGettysWords randomWords{};
    auto directoryName = randomWords.MakeWord();
    auto fileNames = RandomGettysWords().MakeUniqueVector(fileCount);

    auto directoryCreator = DirectoryCreator(directoryName);
    directoryCreator.CreateFiles(fileNames.begin(), fileNames.end());

    auto result = jive::ListDirectory(directoryName);

    CHECK(result.size() == fileNames.size());

    // Directory listing is not required to be ordered.
    std::sort(result.begin(), result.end());
    std::sort(fileNames.begin(), fileNames.end());

    REQUIRE(result == fileNames);
}
