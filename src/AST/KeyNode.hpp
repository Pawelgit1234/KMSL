#pragma once

#include <memory>
#include <vector>
#include <sstream>

#include "AstNode.hpp"
#include "../token/Token.hpp"

namespace kmsl
{
    class KeyNode : public AstNode
    {
    public:
        KeyNode(Token t, std::vector<std::unique_ptr<AstNode>> buttons)
            : token(t), buttonNodes(std::move(buttons)) {}

        std::string toString() const override
        {
            std::ostringstream oss;
            if (token.type == TokenType::HOLD)
                oss << "Hold(";
            else if (token.type == TokenType::RELEASE)
                oss << "Release(";
            else if (token.type == TokenType::PRESS)
                oss << "Press(";
            else
                oss << "UnknownAction(";

            for (size_t i = 0; i < buttonNodes.size(); ++i)
            {
                oss << buttonNodes[i]->toString();
                if (i < buttonNodes.size() - 1)
                    oss << ", ";
            }

            oss << ")";
            return oss.str();
        }

        Token token; // HOLD, RELEASE or PRESS
        std::vector<std::unique_ptr<AstNode>> buttonNodes; // t is at the last element
    };
}
