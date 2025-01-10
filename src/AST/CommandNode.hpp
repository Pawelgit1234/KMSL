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

        std::unique_ptr<AstNode> clone() const override
        {
            return std::unique_ptr<CommandNode>(std::make_unique<CommandNode>(type));
        }

        Token type;
    };
}
