#pragma once

#include <memory>

#include "AstNode.hpp"
#include "../token/Token.hpp"

namespace kmsl
{
	class UnarOpNode : public AstNode
	{
	public:
		UnarOpNode(const Token& oper, std::unique_ptr<AstNode> node)
			: op(oper), operand(std::move(node)) {}

		std::string toString() const override
		{
			return "UnarOpNode(" + op.text + ", " + operand->toString() + ")";
		}

		std::unique_ptr<AstNode> clone() const override
		{
			return std::unique_ptr<UnarOpNode>(std::make_unique<UnarOpNode>(op, operand->clone()));
		}

		Token op;
		std::unique_ptr<AstNode> operand;
	};
}