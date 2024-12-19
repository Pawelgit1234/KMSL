#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <sstream>

#include "../token/Token.hpp"

namespace kmsl
{
	enum class ErrorType
	{
		SYNTAX_ERROR,
		SEMANTIC_ERROR,
		RUNTIME_ERROR,
	};

	struct Error
	{
		ErrorType type;
		std::string message;
		long long pos;

		Error(ErrorType t, const std::string& msg, long long p)
			: type(t), message(msg), pos(p) {}
	};

	class ErrorHandler
	{
	public:
		ErrorHandler() {}

		void setCode(const std::string& c) { code_ = c; }
		void report(const Error& error) { errors_.emplace_back(error); }
		void report(ErrorType t, const std::string& msg, long long p) { errors_.emplace_back(t, msg, p); }
		void clearErrors() { errors_.clear(); }
		int getErrorsCount() { return errors_.size(); }

		void showErrors();

	private:
		std::string stringifyErrorType(ErrorType type);
		std::string getCodeLineFromCode(long long pos);
		int getLineByPosition(long long pos);

		std::vector<Error> errors_;
		std::string code_;
		long long error_pos_;
	};
}