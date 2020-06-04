/**
  * @author Jive Helix (jivehelix@gmail.com)
  * @copyright 2020 Jive Helix
  * Licensed under the MIT license. See LICENSE file.
  */
#pragma once

#include <string>
#include "jive/path.h"
#include <unistd.h>
#include <fstream>

class DirectoryCreator
{
public:
    DirectoryCreator(const std::string &directoryName)
        :
        directoryName_(directoryName)
    {
        jive::path::MakeDirectory(directoryName);
    }

    ~DirectoryCreator()
    {
        for (auto &fileName: this->files_)
        {
            unlink(fileName.data());
        }

        rmdir(this->directoryName_.data());
    }

    template<typename InputIterator>
    void CreateFiles(InputIterator begin, InputIterator end)
    {
        while (begin != end)
        {
            auto fileName = jive::path::Join(this->directoryName_, *begin++);

            std::ofstream outputStream(fileName);
            outputStream << "Test file. Delete me." << std::endl;

            if (!outputStream)
            {
                throw std::runtime_error("Failed to create the test file.");
            }

            this->files_.push_back(fileName);
        }
    }

private:
    std::string directoryName_;
    std::vector<std::string> files_;
};
