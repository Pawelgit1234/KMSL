#pragma once

#define _USE_MATH_DEFINES

#include <variant>
#include <string>
#include <unordered_map>
#include <memory>
#include <sstream>
#include <iostream>
#include <chrono>
#include <ctime>
#include <fstream>
#include <filesystem>
#include <cmath>

#include "../AST/ast.hpp"
#include "../lexer/Lexer.hpp"
#include "../parser/Parser.hpp"
#include "../token/Token.hpp"
#include "../semantic/SemanticAnalyzer.hpp"
#include "../semantic/SymbolTable.hpp"
#include "../io/IoController.hpp"
#include "FileReader.hpp"
#include "../error/ErrorHandler.hpp"

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
		void setCode(const std::string& c, bool auto_visit = false); // auto_visit for DO

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

		void expand_argumented_assigments(BinaryOpNode* node); // a o= n -----> a = a o n (o - operator)
		template <typename NodeType>
		void make_print(NodeType* node, const Token& token); // a + b -----> print a + b (this is just for console)
 
		ErrorHandler error_handler_;
		std::vector<Variable> variables_;
		std::unique_ptr<BlockNode> root_;
		std::vector<Symbol> symbols_; // for semantic-analysis-console

		/* PROGRAMM FLAGS */
		bool break_loop_;
		bool continue_loop_;
		bool exit_program_;
		bool is_printable_; // for console, for example: '> a' or '> 4 * 4' # it will print the answer without "print"
		bool has_errors_;

		/* FLAGS */
		bool logging_enabled_;
		bool console_running_;

		unsigned short deepness_;
		variant temp_var_; // workaround: fix the error with the reference to VAR-FUNC (like YEAR, RANDOM etc.)
	};
}
