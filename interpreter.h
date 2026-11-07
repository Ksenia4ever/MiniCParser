#pragma once

#include "ast.h"

#include <string>
#include <unordered_map>

class Interpreter {
private:
    std::unordered_map<std::string, double> variables;

    double evaluate(const ASTNode* node);
    void execute(const ASTNode* node);

public:
    void run(const ASTNode* root);
};