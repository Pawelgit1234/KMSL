#pragma once

#include <memory>

#include "../AST/ast.hpp"

namespace kmsl
{
    class NodeVisitor {
    public:
        virtual void visit(BlockNode* node) = 0;
        virtual void visit(VariableNode* node) = 0;
        virtual void visit(UnarOpNode* node) = 0;
        virtual void visit(BinaryOpNode* node) = 0;
        virtual void visit(IfNode* node) = 0;
        virtual void visit(ForNode* node) = 0;
        virtual void visit(WhileNode* node) = 0;
        virtual void visit(LiteralNode* node) = 0;
    };
}



