#pragma once

#include <memory>

#include "AstNode.hpp"
#include "../token/Token.hpp"

namespace kmsl
{
	class AssignNode : public AstNode
	{
	public:
		AssignNode(const Token& oper, std::unique_ptr<AstNode> left, std::unique_ptr<AstNode> right)
			: op(oper), leftOperand(std::move(left)), rightOperand(std::move(right)) {}

		Token op;
		std::unique_ptr<AstNode> leftOperand;
		std::unique_ptr<AstNode> rightOperand;
	};
}