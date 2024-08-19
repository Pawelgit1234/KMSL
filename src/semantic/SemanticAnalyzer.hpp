#pragma once

#include <memory>
#include <stdexcept>

#include "../AST/ast.hpp"
#include "SymbolTable.hpp"

namespace kmsl
{
	class SemanticAnalyzer
	{
	public:
		SemanticAnalyzer(std::unique_ptr<BlockNode>& root);

		void analyze();

	private:
		void visitNode(AstNode* node);
		void visit(BlockNode* node);
		void visit(VariableNode* node);
		void visit(UnarOpNode* node);
		void visit(BinaryOpNode* node);
		void visit(IfNode* node);
		void visit(ForNode* node);
		void visit(WhileNode* node);
		void visit(KeyNode* node);
		void visit(MouseNode* node);

		DataType determineType(AstNode* node);
		DataType determineBinaryOpType(BinaryOpNode* node);

		SymbolTable symbol_table_;
		std::unique_ptr<BlockNode>& root_;
	};
}
