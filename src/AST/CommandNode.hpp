#pragma once

#include <memory>

#include "AstNode.hpp"
#include "../token/Token.hpp"

namespace kmsl
{
    class CommandNode : public AstNode // single token node: break, continue, !!
    {
    public:
        CommandNode(TokenType t) : type(t) {}

        std::string toString() const override
        {
            return "CommandNode()";
        }

        TokenType type;
    };
}
