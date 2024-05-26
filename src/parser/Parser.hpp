#pragma once

#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <stdexcept>

#include "../token/Token.hpp"
#include "../token/TokenType.hpp"
#include "../AST/ast.hpp"

namespace kmsl
{
	class Parser
	{
	public:
		Parser(std::vector<Token> tokens);
		~Parser();

		std::unique_ptr<AstNode> parse();
		
	private:
		Token match(std::vector<TokenType> types);
		Token require(std::vector<TokenType> types);

		std::unique_ptr<AstNode> parseExpression();
		std::unique_ptr<UnarOpNode> parseUnar();
		std::unique_ptr<AstNode> parseVariable();
		std::unique_ptr<AstNode> parseTerm();
		std::unique_ptr<IfNode> parseIf();
		std::unique_ptr<ForNode> parseFor();
		std::unique_ptr<WhileNode> parseWhile();

		std::vector<Token> tokens_;
		Token current_token_;
		long long pos_;
	};
}

