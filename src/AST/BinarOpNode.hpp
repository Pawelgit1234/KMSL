#pragma once

#include <memory>

#include "AstNode.hpp"
#include "../token/Token.hpp"

namespace kmsl
{
	class BinarOpNode : public AstNode
	{
	public:
		BinarOpNode(const Token& oper, std::unique_ptr<AstNode> left, std::unique_ptr<AstNode> right)
			: op(oper), leftOperand(std::move(left)), rightOperand(std::move(right)) {}

		std::string toString() const override
		{
			std::string result = "BinaryOpNode(" + op.text + ", " + leftOperand->toString();
			
			if (rightOperand)
				result += ", " + rightOperand->toString() + ")";
			else
				result += ", nullptr)";

			return result;
		}

		std::unique_ptr<AstNode> clone() const override
		{
			return std::make_unique<BinarOpNode>(op, leftOperand->clone(), rightOperand ? rightOperand->clone() : nullptr);
		}

		Token op;
		std::unique_ptr<AstNode> leftOperand;
		std::unique_ptr<AstNode> rightOperand;
	};
}