#pragma once

#include <memory>
#include <stdexcept>

#include "../AST/ast.hpp"
#include "SymbolTable.hpp"
#include "../error/ErrorHandler.hpp"

namespace kmsl
{
	class SemanticAnalyzer
	{
	public:
		SemanticAnalyzer(std::unique_ptr<BlockNode>& root, ErrorHandler& error_handler);

		void analyze();

		void set_symbols(std::vector<Symbol>* symbols) { in_console_ = true; symbols_ = symbols; }

	private:
		void visitNode(AstNode* node);
		void visit(BlockNode* node);
		void visit(VariableNode* node);
		void visit(UnarOpNode* node);
		void visit(BinarOpNode* node);
		void visit(IfNode* node);
		void visit(ForNode* node);
		void visit(WhileNode* node);
		void visit(KeyNode* node);
		void visit(MouseNode* node);
		void visit(CommandNode* node);

		DataType determineType(AstNode* node);
		DataType determineBinaryOpType(BinarOpNode* node);

		ErrorHandler& error_handler_;

		SymbolTable symbol_table_;
		std::unique_ptr<BlockNode>& root_;
		std::vector<Symbol>* symbols_; // for console

		bool inside_loop_;
		unsigned short deepness_;
		bool in_console_;
	};
}
