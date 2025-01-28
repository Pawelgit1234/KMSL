#pragma once

#include <memory>

#include "AstNode.hpp"
#include "../token/Token.hpp"

namespace kmsl
{
    class IfNode : public AstNode
    {
    public:
        IfNode(std::unique_ptr<AstNode> condition, std::unique_ptr<AstNode> thenBranch, std::unique_ptr<AstNode> elseBranch, Token t)
            : conditionNode(std::move(condition)), thenBranchNode(std::move(thenBranch)), elseBranchNode(std::move(elseBranch)), token(t) {}

        std::string toString() const override
        {
            return "If(\n  Condition: " + conditionNode->toString() + ",\n  Then: " + thenBranchNode->toString() + ",\n  Else: " + (elseBranchNode ? elseBranchNode->toString() : "null") + "\n)";
        }

        std::unique_ptr<AstNode> clone() const override
        {
            return std::unique_ptr<IfNode>(std::make_unique<IfNode>(conditionNode->clone(), thenBranchNode->clone(), elseBranchNode ? elseBranchNode->clone() : nullptr, token));
        }

        std::unique_ptr<AstNode> conditionNode;
        std::unique_ptr<AstNode> thenBranchNode;
        std::unique_ptr<AstNode> elseBranchNode;
        Token token; // for error handling
    };
}
