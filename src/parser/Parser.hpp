#pragma once

#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <stdexcept>

#include "../token/Token.hpp"
#include "../token/TokenType.hpp"
#include "../AST/ast.hpp"
#include "../error/ErrorHandler.hpp"

namespace kmsl
{
	class Parser
	{
	public:
		Parser(std::vector<Token> tokens, ErrorHandler& error_handler);
		~Parser();

		std::unique_ptr<BlockNode> parse();
		
	private:
		Token match(std::vector<TokenType> types);
		Token require(std::vector<TokenType> types);
		void removeTokensUntil(std::vector<TokenType> remove_types, std::vector<TokenType> stop_types);

		std::unique_ptr<AstNode> parseLine();
		std::unique_ptr<AstNode> parseVariable();
		std::unique_ptr<MouseNode> parseMouse();
		std::unique_ptr<BinarOpNode> parseTypeAndScroll();
		std::unique_ptr<BinarOpNode> parseFileAndDir();
		std::vector<std::unique_ptr<AstNode>> parseArguments();
		
		// constructions
		std::unique_ptr<IfNode> parseIf();
		std::unique_ptr<ForNode> parseFor();
		std::unique_ptr<WhileNode> parseWhile();

		// formula
		std::unique_ptr<AstNode> parseFactor();
		std::unique_ptr<AstNode> parseTerm();
		std::unique_ptr<AstNode> parseExpression();

		// extra
		bool checkAllPars();

		// fields
		ErrorHandler& error_handler_;

		std::vector<Token> tokens_;
		Token current_token_;
		long long pos_;
	};
}

