#pragma once

#include <memory>

#include "AstNode.hpp"
#include "../token/Token.hpp"

namespace kmsl
{
    class CommandNode : public AstNode // single token node: break, continue, !!
    {
    public:
        CommandNode(Token t) : type(t) {}

        std::string toString() const override
        {
            return "CommandNode(" + type.text + ")";
        }

        Token type;
    };
}
