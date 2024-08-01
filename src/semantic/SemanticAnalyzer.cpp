#include "SemanticAnalyzer.hpp"


#include <iostream>
namespace kmsl
{
	SemanticAnalyzer::SemanticAnalyzer() : symbol_table_() {}

	void SemanticAnalyzer::analyze(std::unique_ptr<BlockNode>& root)
	{
		visit(root.get());
	}

	void SemanticAnalyzer::visitNode(AstNode* node)
	{
		if (auto blockNode = dynamic_cast<BlockNode*>(node))
			visit(blockNode);
		else if (auto variableNode = dynamic_cast<VariableNode*>(node))
			visit(variableNode);
		else if (auto unarOpNode = dynamic_cast<UnarOpNode*>(node))
			visit(unarOpNode);
		else if (auto binaryOpNode = dynamic_cast<BinaryOpNode*>(node))
			visit(binaryOpNode);
		else if (auto ifNode = dynamic_cast<IfNode*>(node))
			visit(ifNode);
		else if (auto forNode = dynamic_cast<ForNode*>(node))
			visit(forNode);
		else if (auto whileNode = dynamic_cast<WhileNode*>(node))
			visit(whileNode);
	}


	void SemanticAnalyzer::visit(BlockNode* node)
	{
		symbol_table_.enterScope();
		for (auto& stmt : node->getStatements())
			visitNode(stmt.get());

		symbol_table_.exitScope();
	}

	void SemanticAnalyzer::visit(VariableNode* node)
	{
		if (!symbol_table_.isDeclared(node->token.text))
			throw std::runtime_error("The variable" + node->token.text + "is not existing!");
	}

	void SemanticAnalyzer::visit(UnarOpNode* node)
	{
		visitNode(node->operand.get());
	}

	void SemanticAnalyzer::visit(BinaryOpNode* node)
	{
		
	}

	void SemanticAnalyzer::visit(IfNode* node)
	{
		
	}

	void SemanticAnalyzer::visit(ForNode* node)
	{
		
	}

	void SemanticAnalyzer::visit(WhileNode* node)
	{
		
	}
}