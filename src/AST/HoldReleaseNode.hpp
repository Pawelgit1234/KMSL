#pragma once

#include <memory>
#include <vector>
#include <sstream>

#include "AstNode.hpp"
#include "../token/Token.hpp"

namespace kmsl
{
    class HoldReleaseNode : public AstNode
    {
    public:
        HoldReleaseNode(TokenType t, std::vector<std::unique_ptr<AstNode>> buttons)
            : token_type(t), buttonsNode(std::move(buttons)) {}

        std::string toString() const override
        {
            std::ostringstream oss;
            if (token_type == TokenType::HOLD)
                oss << "Hold(";
            else if (token_type == TokenType::RELEASE)
                oss << "Release(";
            else
                oss << "UnknownAction(";

            for (size_t i = 0; i < buttonsNode.size(); ++i)
            {
                oss << buttonsNode[i]->toString();
                if (i < buttonsNode.size() - 1)
                    oss << ", ";
            }

            oss << ")";
            return oss.str();
        }

        TokenType token_type; // HOLD or RELEASE
        std::vector<std::unique_ptr<AstNode>> buttonsNode;
    };
}
