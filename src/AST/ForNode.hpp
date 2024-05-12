#pragma once

#include <memory>

#include "AstNode.hpp"
#include "../token/Token.hpp"

namespace kmsl
{
    class ForNode : public AstNode
    {
    public:
        ForNode(std::unique_ptr<AstNode> initializer, std::unique_ptr<AstNode> condition, std::unique_ptr<AstNode> increment, std::unique_ptr<AstNode> body)
            : initializerNode(std::move(initializer)), conditionNode(std::move(condition)), incrementNode(std::move(increment)), bodyNode(std::move(body)) {}

        std::unique_ptr<AstNode> initializerNode;
        std::unique_ptr<AstNode> conditionNode;
        std::unique_ptr<AstNode> incrementNode;
        std::unique_ptr<AstNode> bodyNode;
    };
}
