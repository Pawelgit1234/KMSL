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
			: var(variable) {}

		Token var;
	};
}