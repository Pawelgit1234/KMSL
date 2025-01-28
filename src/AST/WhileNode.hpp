#pragma once

#include <memory>

#include "AstNode.hpp"
#include "../token/Token.hpp"

namespace kmsl
{
    class WhileNode : public AstNode
    {
    public:
        WhileNode(std::unique_ptr<AstNode> condition, std::unique_ptr<AstNode> body, Token t)
            : conditionNode(std::move(condition)), bodyNode(std::move(body)), token(t) {}

        std::string toString() const override
        {
            return "While(\n  Condition: " + conditionNode->toString() + ",\n  Body: " + bodyNode->toString() + "\n)";
        }

        std::unique_ptr<AstNode> clone() const override
        {
            return std::unique_ptr<WhileNode>(std::make_unique<WhileNode>(conditionNode->clone(), bodyNode->clone(), token));
        }

        std::unique_ptr<AstNode> conditionNode;
        std::unique_ptr<AstNode> bodyNode;
        Token token; // for error handling
    };
}
