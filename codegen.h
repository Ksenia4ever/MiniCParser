#pragma once

#include "ast.h"

#include <ostream>
#include <string>

class CodeGenerator {
private:
    std::ostream& output;
    int tempCounter = 0;

    std::string generateExpression(const ASTNode* node);
    void generateStatement(const ASTNode* node);

    std::string newTemp();

public:
    explicit CodeGenerator(std::ostream& out);

    void generate(const ASTNode* root);
};