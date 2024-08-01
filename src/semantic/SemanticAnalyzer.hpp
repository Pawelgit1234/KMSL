#pragma once

#include <memory>
#include <stdexcept>

#include "../AST/ast.hpp"
#include "../NodeVisitor.hpp"
#include "SymbolTable.hpp"

namespace kmsl
{
	class SemanticAnalyzer : public NodeVisitor
	{
	public:
		SemanticAnalyzer();

		void analyze(std::unique_ptr<BlockNode>& root);

	private:
		void visitNode(AstNode* node);

		void visit(BlockNode* node) override;
		void visit(VariableNode* node) override;
		void visit(UnarOpNode* node) override;
		void visit(BinaryOpNode* node) override;
		void visit(IfNode* node) override;
		void visit(ForNode* node) override;
		void visit(WhileNode* node) override;
		void visit(LiteralNode* node) override {}

		SymbolTable symbol_table_;
	};
}
