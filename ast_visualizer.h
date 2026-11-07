
#pragma once

#include "ast.h"

#include <ostream>
#include <string>

class ASTVisualizer {
private:
    std::ostream& output;
    int nodeCounter = 0;

    int writeNode(const ASTNode* node);
    std::string nodeLabel(const ASTNode* node) const;
    static std::string escape(const std::string& text);

public:
    explicit ASTVisualizer(std::ostream& out);

    void generate(const ASTNode* root);
};
