#pragma once

#include <memory>

#include "AstNode.hpp"
#include "../token/Token.hpp"

namespace kmsl
{
    class IfNode : public AstNode
    {
    public:
        IfNode(std::unique_ptr<AstNode> condition, std::unique_ptr<AstNode> thenBranch, std::unique_ptr<AstNode> elseBranch)
            : conditionNode(std::move(condition)), thenBranchNode(std::move(thenBranch)), elseBranchNode(std::move(elseBranch)) {}

        std::unique_ptr<AstNode> conditionNode;
        std::unique_ptr<AstNode> thenBranchNode;
        std::unique_ptr<AstNode> elseBranchNode;
    };
}
