#pragma once

#include <variant>
#include <string>
#include <unordered_map>
#include <memory>
#include <sstream>
#include <iostream>

#include "../AST/ast.hpp"
#include "../lexer/Lexer.hpp"
#include "../parser/Parser.hpp"
#include "../token/Token.hpp"
#include "../semantic/SemanticAnalyzer.hpp"
#include "../semantic/SymbolTable.hpp"
#include "../io/IoController.hpp"
#include "FileReader.hpp"

namespace kmsl
{
	using variant = std::variant<bool, int, float, std::string>;

	struct Variable
	{
		variant value;
		std::string name;
		unsigned short deepness;
	};

	class Interpreter
	{
	public:
		Interpreter();
		~Interpreter();

		void execute();
		void runConsole();

		void setLoggingEnabled(bool logging_enabled) { logging_enabled_ = logging_enabled; }
		void setCode(const std::string& code);

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
		variant visit(CommandNode* node);

		std::vector<Variable> variables_;
		std::unique_ptr<BlockNode> root_;
		std::vector<Symbol> symbols_; // for semantic-analysis-console

		bool break_loop_;
		bool continue_loop_;
		bool exit_program_;

		bool logging_enabled_;
		bool console_running_;

		unsigned short deepness_;
	};
}
