#pragma once

#include <memory>

#include "../AST/ast.hpp"

namespace kmsl
{
	class SemanticAnalyzer
	{
	public:
		void analyze(std::unique_ptr<BlockNode>& root);

	private:

	};
}
