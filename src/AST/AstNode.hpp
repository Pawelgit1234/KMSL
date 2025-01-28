#pragma once

#include <string>
#include <memory>

namespace kmsl
{
	class AstNode
	{
	public:
		virtual ~AstNode() {}
		virtual std::string toString() const { return "Base Class"; }
		virtual std::unique_ptr<AstNode> clone() const { return std::make_unique<AstNode>(); };
	};
}