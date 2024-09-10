#pragma once

#include <fstream>
#include <string>
#include <stdexcept>

namespace kmsl
{
    namespace utils
    {
        class FileReader
        {
        public:
            FileReader(const std::string& filePath) : filePath_(filePath) {}

            std::string read();

        private:
            std::string replaceEscapedNewlines(const std::string& input);

            std::string filePath_;
        };
    }
}