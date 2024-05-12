#pragma once

#include <vector>
#include <memory>

#include "AstNode.hpp"

namespace kmsl
{
    class BlockNode : public AstNode
    {
    public:
        BlockNode() {}

        void addStatement(std::unique_ptr<AstNode> statement)
        {
            statements_.push_back(std::move(statement));
        }

        const std::vector<std::unique_ptr<AstNode>>& getStatements() const
        {
            return statements_;
        }

    private:
        std::vector<std::unique_ptr<AstNode>> statements_;
    };
}
