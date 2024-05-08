#include "Lexer.hpp"

namespace kmsl
{
	Lexer::Lexer(const std::string& code)
		: code_(code), pos_(0) {}

	Lexer::~Lexer()
	{
		code_.clear();
		tokens_.clear();
	}

	std::vector<Token> Lexer::scanTokens()
	{
		return std::vector<Token>();
	}


}