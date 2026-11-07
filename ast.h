#pragma once

#include <memory>
#include <string>
#include <vector>

enum class NodeType {
    Number,
    Variable,
    BinaryOperation,
    UnaryOperation,
    Declaration,
    Assignment,
    Print,
    Block
};

struct ASTNode {
    NodeType type;
    std::string value;
    std::vector<std::unique_ptr<ASTNode>> children;

    ASTNode(NodeType nodeType, const std::string& nodeValue = "")
        : type(nodeType), value(nodeValue) {}

    void addChild(std::unique_ptr<ASTNode> child) {
        children.push_back(std::move(child));
    }
};

using NodePtr = std::unique_ptr<ASTNode>;