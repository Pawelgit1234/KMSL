#pragma once

#include <string>

namespace kmsl
{
	class AstNode
	{
	public:
		virtual ~AstNode() {}
		virtual std::string toString() const { return "Base Class"; }
	};
}