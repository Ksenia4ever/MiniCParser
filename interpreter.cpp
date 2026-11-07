#include "interpreter.h"

#include <cmath>
#include <iostream>
#include <stdexcept>

double Interpreter::evaluate(const ASTNode* node) {
    if (node == nullptr) {
        throw std::runtime_error("Invalid expression");
    }

    switch (node->type) {
        case NodeType::Number:
            return std::stod(node->value);

        case NodeType::Variable: {
            auto it = variables.find(node->value);

            if (it == variables.end()) {
                throw std::runtime_error(
                    "Undefined variable: " + node->value
                );
            }

            return it->second;
        }

        case NodeType::UnaryOperation: {
            double operand = evaluate(node->children.at(0).get());

            if (node->value == "-") {
                return -operand;
            }

            throw std::runtime_error(
                "Unknown unary operator: " + node->value
            );
        }

        case NodeType::BinaryOperation: {
            double left = evaluate(node->children.at(0).get());
            double right = evaluate(node->children.at(1).get());

            if (node->value == "+") {
                return left + right;
            }

            if (node->value == "-") {
                return left - right;
            }

            if (node->value == "*") {
                return left * right;
            }

            if (node->value == "/") {
                if (right == 0.0) {
                    throw std::runtime_error("Division by zero");
                }

                return left / right;
            }

            throw std::runtime_error(
                "Unknown binary operator: " + node->value
            );
        }

        default:
            throw std::runtime_error(
                "Unsupported expression node"
            );
    }
}

void Interpreter::execute(const ASTNode* node) {
    if (node == nullptr) {
        return;
    }

    switch (node->type) {
        case NodeType::Block:
            for (const auto& child : node->children) {
                execute(child.get());
            }
            break;

        case NodeType::Declaration: {
            const std::string& name = node->value;

            if (variables.find(name) != variables.end()) {
                throw std::runtime_error(
                    "Variable already declared: " + name
                );
            }

            double value = 0.0;

            if (!node->children.empty()) {
                value = evaluate(node->children.at(0).get());
            }

            variables[name] = value;
            break;
        }

        case NodeType::Assignment: {
            const std::string& name = node->value;

            auto it = variables.find(name);

            if (it == variables.end()) {
                throw std::runtime_error(
                    "Assignment to undefined variable: " + name
                );
            }

            it->second = evaluate(node->children.at(0).get());
            break;
        }

        case NodeType::Print: {
            double value = evaluate(node->children.at(0).get());
            std::cout << "Output: " << value << '\n';
            break;
        }

        default:
            throw std::runtime_error(
                "Unsupported statement node"
            );
    }
}

void Interpreter::run(const ASTNode* root) {
    variables.clear();
    execute(root);
}