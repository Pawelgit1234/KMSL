#pragma once

#include <string>
#include <vector>

#include "../token/Token.hpp"

namespace kmsl
{
	class Lexer
	{
	public:
		Lexer(const std::string& code);
		~Lexer();

		std::vector<Token> scanTokens();

	private:
		std::string code_;
		long long pos_;
		std::vector<Token> tokens_;
	};
}


