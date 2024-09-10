#include "FileReader.hpp"

namespace kmsl
{
	namespace utils
	{
		std::string FileReader::read() 
        {
            std::ifstream file(filePath_);
            if (!file.is_open())
                throw std::runtime_error("Failed to open file: " + filePath_);

            std::string content;
            std::string line;
            while (std::getline(file, line))
                content += line + '\n';

            std::string formattedString = replaceEscapedNewlines(content);

            file.close();
            return formattedString;
        }

        std::string FileReader::replaceEscapedNewlines(const std::string& input)
        {
            std::string result;
            result.reserve(input.size());

            for (size_t i = 0; i < input.size(); ++i) {
                if (input[i] == '\\' && i + 1 < input.size() && input[i + 1] == 'n') {
                    result += '\n';
                    ++i;
                }
                else {
                    result += input[i];
                }
            }

            return result;
        }

	}
}