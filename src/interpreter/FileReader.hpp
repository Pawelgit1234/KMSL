#pragma once

#include <fstream>
#include <string>
#include <stdexcept>

namespace kmsl
{
    class FileReader
    {
    public:
        FileReader(const std::string& filePath) : filePath_(filePath) {}

        std::string read();

        static std::string replaceEscapedNewlines(const std::string& input);

    private:
        std::string filePath_;
    };
}