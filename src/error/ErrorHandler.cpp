#include "ErrorHandler.hpp"

void kmsl::ErrorHandler::showErrors()
{
	std::cerr << std::endl << std::to_string(getErrorsCount()) << " error/-s was found..." << std::endl;

	for (const Error& e : errors_)
	{
		std::string code_line = getCodeLineFromCode(e.pos);

		std::cerr << std::endl << '[' << stringifyErrorType(e.type) << "][" << getLineByPosition(e.pos) << "] " << e.message << std::endl;
		std::cerr << code_line << std::endl;

		for (int i = 1; i < error_pos_; i++)
			std::cerr << '~';

		std::cerr << '^';

		for (int i = 0; i < std::abs(static_cast<long long>(code_line.size()) - error_pos_); i++)
			std::cerr << '~';

		std::cerr << std::endl;
	}
}

std::string kmsl::ErrorHandler::stringifyErrorType(ErrorType type)
{
	switch (type)
	{
	case kmsl::ErrorType::SYNTAX_ERROR:
		return "SYNTAX_ERROR";
	case kmsl::ErrorType::SEMANTIC_ERROR:
		return "SEMANTIC_ERROR";
	case kmsl::ErrorType::RUNTIME_ERROR:
		return "RUNTIME_ERROR";
	}
}

std::string kmsl::ErrorHandler::getCodeLineFromCode(long long pos)
{
	long long start = code_.rfind('\n', pos); // finds the previous \n
	if (start == std::string::npos)
		start = 0;
	else
		start++;

	long long end = code_.find('\n', pos); // finds the next \n
	if (end == std::string::npos)
		end = code_.size();

	error_pos_ = pos - start;
	return code_.substr(start, end - start);
}

int kmsl::ErrorHandler::getLineByPosition(long long pos)
{
	std::vector<std::string> lines;
	std::istringstream stream(code_);
	std::string line;

	while (std::getline(stream, line))
		lines.push_back(line + "\n");

	int current_position = 0;
	int line_number = 0;

	for (const auto& l : lines)
	{
		if (pos >= current_position && pos < current_position + l.size())
			break;
		current_position += l.size();
		line_number++;
	}

	return line_number + 1;
}
