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
        KeyNode(TokenType t, std::vector<std::unique_ptr<AstNode>> buttons)
            : token_type(t), buttonNodes(std::move(buttons)) {}

        std::string toString() const override
        {
            std::ostringstream oss;
            if (token_type == TokenType::HOLD)
                oss << "Hold(";
            else if (token_type == TokenType::RELEASE)
                oss << "Release(";
            else if (token_type == TokenType::PRESS)
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

        TokenType token_type; // HOLD, RELEASE or PRESS
        std::vector<std::unique_ptr<AstNode>> buttonNodes;
    };
}
