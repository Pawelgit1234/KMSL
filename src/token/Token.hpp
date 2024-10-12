#pragma once

#include <string>

#include "TokenType.hpp"

namespace kmsl
{
	struct Token
	{
		Token(TokenType t, const std::string& txt, long long p)
			: type(t), text(txt), pos(p) {}

		Token() : type(TokenType::INVALID), text(""), pos(0) {}

		TokenType type;
		std::string text;
		long long pos;
	};
}