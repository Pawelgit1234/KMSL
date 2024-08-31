#pragma once

#include <variant>
#include <string>
#include <unordered_map>
#include <memory>
#include <sstream>
#include <iostream>

#include "AST/ast.hpp"
#include "lexer/Lexer.hpp"
#include "parser/Parser.hpp"
#include "token/Token.hpp"
#include "semantic/SemanticAnalyzer.hpp"
#include "semantic/SymbolTable.hpp"
#include "io/IoController.hpp"

using variant = std::variant<bool, int, float, std::string>;

namespace kmsl
{
	class Intepreter
	{
	public:
		Intepreter(const std::string& code);
		~Intepreter();

		void execute();

	private:
		variant visitNode(AstNode* node);
		variant visit(BlockNode* node);
		variant& visit(VariableNode* node);
		variant visit(UnarOpNode* node);
		variant visit(BinaryOpNode* node);
		variant visit(IfNode* node);
		variant visit(ForNode* node);
		variant visit(WhileNode* node);
		variant visit(LiteralNode* node);
		variant visit(KeyNode* node);
		variant visit(MouseNode* node);

		std::unordered_map<std::string, variant> variables_;
		std::unique_ptr<BlockNode> root_;
	};
}
