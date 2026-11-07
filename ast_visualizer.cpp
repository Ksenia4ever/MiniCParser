
#include "ast_visualizer.h"

#include <stdexcept>
#include <string>

ASTVisualizer::ASTVisualizer(std::ostream& out)
    : output(out) {
}

// Экранирование специальных символов для Graphviz DOT
std::string ASTVisualizer::escape(const std::string& text) {
    std::string escaped;

    for (char c : text) {
        switch (c) {
            case '\\':
                escaped += "\\\\";
                break;

            case '"':
                escaped += "\\\"";
                break;

            case '\n':
                escaped += "\\n";
                break;

            case '\r':
                break;

            default:
                escaped += c;
                break;
        }
    }

    return escaped;
}

// Формирование подписи узла AST
std::string ASTVisualizer::nodeLabel(const ASTNode* node) const {
    if (node == nullptr) {
        return "null";
    }

    std::string label;

    switch (node->type) {
        case NodeType::Number:
            label = "Number";
            break;

        case NodeType::Variable:
            label = "Variable";
            break;

        case NodeType::BinaryOperation:
            label = "Binary Operation";
            break;

        case NodeType::UnaryOperation:
            label = "Unary Operation";
            break;

        case NodeType::Declaration:
            label = "Declaration";
            break;

        case NodeType::Assignment:
            label = "Assignment";
            break;

        case NodeType::Print:
            label = "Print";
            break;

        case NodeType::Block:
            label = "Block";
            break;
    }

    // Настоящий перенос строки, а не текст "\n"
    if (!node->value.empty()) {
        label += '\n';
        label += node->value;
    }

    return label;
}

// Рекурсивное создание узлов и связей графа
int ASTVisualizer::writeNode(const ASTNode* node) {
    if (node == nullptr) {
        throw std::runtime_error(
            "Cannot visualize a null AST node"
        );
    }

    int currentId = nodeCounter++;

    output << "    node" << currentId
           << " [label=\""
           << escape(nodeLabel(node))
           << "\"];\n";

    for (const auto& child : node->children) {
        if (child == nullptr) {
            continue;
        }

        int childId = writeNode(child.get());

        output << "    node" << currentId
               << " -> node" << childId
               << ";\n";
    }

    return currentId;
}

// Генерация полного DOT-графа
void ASTVisualizer::generate(const ASTNode* root) {
    if (root == nullptr) {
        throw std::runtime_error(
            "Cannot visualize an empty AST"
        );
    }

    nodeCounter = 0;

    output << "digraph AST {\n";

    output << "    rankdir=TB;\n";

    output << "    graph ["
              "bgcolor=\"white\", "
              "pad=\"0.4\", "
              "nodesep=\"0.35\", "
              "ranksep=\"0.6\""
              "];\n";

    output << "    node ["
              "shape=box, "
              "style=\"rounded,filled\", "
              "fillcolor=\"#E8F1FF\", "
              "color=\"#3778BF\", "
              "fontname=\"Arial\", "
              "fontsize=12, "
              "margin=\"0.18,0.12\""
              "];\n";

    output << "    edge ["
              "color=\"#64748B\", "
              "arrowsize=0.7"
              "];\n\n";

    writeNode(root);

    output << "}\n";
}
