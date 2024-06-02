#pragma once

#include <memory>

#include "AstNode.hpp"
#include "../token/Token.hpp"

namespace kmsl
{
    class WhileNode : public AstNode
    {
    public:
        WhileNode(std::unique_ptr<AstNode> condition, std::unique_ptr<AstNode> body)
            : conditionNode(std::move(condition)), bodyNode(std::move(body)) {}

        std::string toString() const override
        {
            return "While(\n  Condition: " + conditionNode->toString() + ",\n  Body: " + bodyNode->toString() + "\n)";
        }

        std::unique_ptr<AstNode> conditionNode;
        std::unique_ptr<AstNode> bodyNode;
    };
}
