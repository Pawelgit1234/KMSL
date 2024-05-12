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

        Token token;
    };
}
