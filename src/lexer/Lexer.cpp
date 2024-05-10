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
		while (pos_ < code_.length())
			scanToken();

		tokens_.push_back(Token(TokenType::LINE_END, "\n", pos_ + 1));

		return tokens_;
	}

	void Lexer::scanToken()
	{
		for (std::pair<std::string, TokenType> token_type : token_list)
		{
			std::regex pattern('^' + token_type.first);
			std::smatch match;
			std::string code = code_.substr(pos_);

			if (std::regex_search(code, match, pattern))
			{
				pos_ += match.str().length();
				Token token(token_type.second, match.str(), pos_);
				if (token.type != TokenType::SPACE)
					tokens_.push_back(token);
			}
		}
	}
}