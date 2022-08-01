#pragma once

#include "parser.hpp"

class NodeVisitor
{
public:
    virtual ~NodeVisitor() = default;
    virtual int visit(ASTNode *node) = 0;
};

class Interpreter : public NodeVisitor
{
private:
    ASTNode *ast;

public:
    Interpreter(ASTNode *root)
    {
        ast = root;
    }

    int interpret()
    {
        return visit(ast);
    }

    int visit(ASTNode *);
};