#pragma once

#include <memory>
#include "AstNode.hpp"
#include "../token/Token.hpp"

namespace kmsl
{
    class MouseNode : public AstNode
    {
    public:
        MouseNode(TokenType t, std::unique_ptr<AstNode> x, std::unique_ptr<AstNode> y, std::unique_ptr<AstNode> time)
            : token_type(t), xNode(std::move(x)), yNode(std::move(y)), tNode(std::move(time)) {}

        std::string toString() const override
        {
            std::string result;

            switch (token_type)
            {
            case TokenType::MOVE:
                result = "Move(" + xNode->toString() + ", " + yNode->toString();
                if (tNode)
                    result += ", " + tNode->toString();
                result += ")";
                break;

            case TokenType::DMOVE:
                result = "DMove(" + xNode->toString() + ", " + yNode->toString();
                if (tNode)
                    result += ", " + tNode->toString();
                result += ")";
                break;

            default:
                result = "UnknownMouseAction()";
                break;
            }

            return result;
        }

        TokenType token_type; // move or dmove 
        std::unique_ptr<AstNode> xNode;
        std::unique_ptr<AstNode> yNode;
        std::unique_ptr<AstNode> tNode;
    };
}
