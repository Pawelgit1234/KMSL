#pragma once

#include <memory>

#include "AstNode.hpp"
#include "../token/Token.hpp"

namespace kmsl
{
	class BinaryOpNode : public AstNode
	{
	public:
		BinaryOpNode(const Token& oper, std::unique_ptr<AstNode> left, std::unique_ptr<AstNode> right)
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

		Token op;
		std::unique_ptr<AstNode> leftOperand;
		std::unique_ptr<AstNode> rightOperand;
	};
}