#include "Parser.hpp"

namespace kmsl
{
	Parser::Parser(std::vector<Token> tokens)
		: tokens_(tokens), pos_(0) {}

	Parser::~Parser()
	{
		tokens_.clear();
	}

	void Parser::parse()
	{
	}
}