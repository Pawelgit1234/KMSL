#pragma once

#include <memory>
#include <vector>
#include <sstream>

#include "AstNode.hpp"
#include "../token/Token.hpp"

namespace kmsl
{
    class PressNode : public AstNode
    {
    public:
        PressNode(std::vector<std::unique_ptr<AstNode>> buttons, std::unique_ptr<AstNode> time = nullptr)
            : buttonsNode(std::move(buttons)), tNode(std::move(time)) {}

        std::string toString() const override
        {
            std::ostringstream oss;
            oss << "PressNode(";

            for (size_t i = 0; i < buttonsNode.size(); ++i)
            {
                oss << buttonsNode[i]->toString();
                if (i < buttonsNode.size() - 1)
                    oss << ", "; 
            }

            if (tNode)
                oss << ", " << tNode->toString();

            oss << ')';
            return oss.str();
        }

        std::vector<std::unique_ptr<AstNode>> buttonsNode;
        std::unique_ptr<AstNode> tNode;
    };
}
