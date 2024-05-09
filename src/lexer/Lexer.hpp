#pragma once

#include <string>
#include <vector>
#include <regex>

#include "../token/Token.hpp"
#include "../token/TokenType.hpp"

namespace kmsl
{
	class Lexer
	{
	public:
		Lexer(const std::string& code);
		~Lexer();

		std::vector<Token> scanTokens();

	private:
		void scanToken();

		std::string code_;
		long long pos_;
		std::vector<Token> tokens_;
	};
}


