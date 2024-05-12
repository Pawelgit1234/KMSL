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


		Token op;
		std::unique_ptr<AstNode> operand;
	};
}