#pragma once

#include <memory>

#include "AstNode.hpp"
#include "../token/Token.hpp"

namespace kmsl
{
    class ForNode : public AstNode
    {
    public:
        ForNode(std::unique_ptr<AstNode> initializer, std::unique_ptr<AstNode> condition, std::unique_ptr<AstNode> increment, std::unique_ptr<AstNode> body, Token t)
            : initializerNode(std::move(initializer)), conditionNode(std::move(condition)), incrementNode(std::move(increment)), bodyNode(std::move(body)), token(t) {}

        std::string toString() const override
        {
            return "For(\n  Init: " + initializerNode->toString() + ",\n  Condition: " + conditionNode->toString() + ",\n  Increment: " + incrementNode->toString() + ",\n  Body: " + bodyNode->toString() + "\n)";
        }

        std::unique_ptr<AstNode> clone() const override
        {
            return std::unique_ptr<ForNode>(std::make_unique<ForNode>(initializerNode->clone(), conditionNode->clone(), incrementNode->clone(), bodyNode->clone(), token));
        }

        std::unique_ptr<AstNode> initializerNode;
        std::unique_ptr<AstNode> conditionNode;
        std::unique_ptr<AstNode> incrementNode;
        std::unique_ptr<AstNode> bodyNode;
        Token token; // for error handling
    };
}
