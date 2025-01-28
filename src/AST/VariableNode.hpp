#pragma once

#include <memory>

#include "AstNode.hpp"
#include "../token/Token.hpp"

namespace kmsl
{
	class VariableNode : public AstNode
	{
	public:
		VariableNode(const Token& variable)
			: token(variable) {}

		std::string toString() const override
		{
			return "Variable(" + token.text + ")";
		}

		std::unique_ptr<AstNode> clone() const override
		{
			return std::unique_ptr<VariableNode>(std::make_unique<VariableNode>(token));
		}

		Token token;
	};
}