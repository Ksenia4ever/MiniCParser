
#include "codegen.h"

#include <stdexcept>
#include <string>

CodeGenerator::CodeGenerator(std::ostream& out)
    : output(out) {
}

std::string CodeGenerator::newTemp() {
    ++tempCounter;
    return "t" + std::to_string(tempCounter);
}

std::string CodeGenerator::generateExpression(const ASTNode* node) {
    if (node == nullptr) {
        throw std::runtime_error("Invalid expression");
    }

    switch (node->type) {
        case NodeType::Number:
            return node->value;

        case NodeType::Variable:
            return node->value;

        case NodeType::UnaryOperation: {
            std::string operand =
                generateExpression(node->children.at(0).get());

            std::string temp = newTemp();

            if (node->value == "-") {
                output << temp << " = -" << operand << '\n';
                return temp;
            }

            throw std::runtime_error(
                "Unsupported unary operator: " + node->value
            );
        }

        case NodeType::BinaryOperation: {
            std::string left =
                generateExpression(node->children.at(0).get());

            std::string right =
                generateExpression(node->children.at(1).get());

            std::string temp = newTemp();

            output << temp << " = "
                   << left << " "
                   << node->value << " "
                   << right << '\n';

            return temp;
        }

        default:
            throw std::runtime_error(
                "Unsupported expression node"
            );
    }
}

void CodeGenerator::generateStatement(const ASTNode* node) {
    if (node == nullptr) {
        return;
    }

    switch (node->type) {
        case NodeType::Block:
            for (const auto& child : node->children) {
                generateStatement(child.get());
            }
            break;

        case NodeType::Declaration: {
            output << "DECLARE " << node->value << '\n';

            if (!node->children.empty()) {
                std::string result =
                    generateExpression(node->children.at(0).get());

                output << node->value
                       << " = " << result << '\n';
            }
            break;
        }

        case NodeType::Assignment: {
            std::string result =
                generateExpression(node->children.at(0).get());

            output << node->value
                   << " = " << result << '\n';
            break;
        }

        case NodeType::Print: {
            std::string result =
                generateExpression(node->children.at(0).get());

            output << "PRINT " << result << '\n';
            break;
        }

        default:
            throw std::runtime_error(
                "Unsupported statement node"
            );
    }
}

void CodeGenerator::generate(const ASTNode* root) {
    tempCounter = 0;

    output << "# Three-Address Code (TAC)\n";
    generateStatement(root);
}
