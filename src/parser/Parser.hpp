#pragma once

#include <vector>

#include "../token/Token.hpp"
#include "../token/TokenType.hpp"


namespace kmsl
{
	class Parser
	{
	public:
		Parser(std::vector<Token> tokens);
		~Parser();

		void parse();
		
	private:
		std::vector<Token> tokens_;
		long long pos_;
	};
}

