#pragma once

#include "AstNode.hpp"
#include "../token/Token.hpp"

namespace kmsl
{
    class LiteralNode : public AstNode
    {
    public:
        LiteralNode(Token t)
            : token(t) {}

        std::string toString() const override
        {
            return "Literal(" + token.text + ")";
        }

        std::unique_ptr<AstNode> clone() const override
        {
            return std::unique_ptr<LiteralNode>(std::make_unique<LiteralNode>(token));
        }

        Token token;
    };
}
