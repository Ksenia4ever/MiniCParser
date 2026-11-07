
#include <cstdio>
#include <exception>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

#include "ast.h"
#include "interpreter.h"
#include "codegen.h"
#include "ast_visualizer.h"

extern int yyparse();
extern FILE* yyin;
extern int yylineno;
extern ASTNode* root;

void printAST(const ASTNode* node, int depth = 0) {
    if (node == nullptr) {
        return;
    }

    std::string indent(depth * 2, ' ');
    std::cout << indent;

    switch (node->type) {
        case NodeType::Number:
            std::cout << "Number";
            break;

        case NodeType::Variable:
            std::cout << "Variable";
            break;

        case NodeType::BinaryOperation:
            std::cout << "BinaryOperation";
            break;

        case NodeType::UnaryOperation:
            std::cout << "UnaryOperation";
            break;

        case NodeType::Declaration:
            std::cout << "Declaration";
            break;

        case NodeType::Assignment:
            std::cout << "Assignment";
            break;

        case NodeType::Print:
            std::cout << "Print";
            break;

        case NodeType::Block:
            std::cout << "Block";
            break;
    }

    if (!node->value.empty()) {
        std::cout << ": " << node->value;
    }

    std::cout << '\n';

    for (const auto& child : node->children) {
        printAST(child.get(), depth + 1);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0]
                  << " <source-file>\n";
        return 1;
    }

    FILE* input = fopen(argv[1], "r");

    if (input == nullptr) {
        std::cerr << "Error: cannot open file "
                  << argv[1] << '\n';
        return 1;
    }

    yyin = input;
    yylineno = 1;

    std::cout << "MiniC Parser\n";
    std::cout << "Source file: " << argv[1] << "\n\n";

    int parseResult = yyparse();

    fclose(input);

    std::unique_ptr<ASTNode> ast(root);
    root = nullptr;

    if (parseResult != 0 || ast == nullptr) {
        std::cerr << "\nParsing failed.\n";
        return 1;
    }

    std::cout << "Syntax analysis completed successfully!\n\n";

    std::cout << "Abstract Syntax Tree:\n";
    printAST(ast.get());

    try {
        // 1. Выполнение программы
        std::cout << "\nProgram execution:\n";

        Interpreter interpreter;
        interpreter.run(ast.get());

        std::cout << "\nExecution completed successfully!\n";

        // 2. Генерация трёхадресного кода
        std::cout << "\nGenerated Three-Address Code:\n";

        CodeGenerator generator(std::cout);
        generator.generate(ast.get());

        std::ofstream tacFile("output.tac");

        if (!tacFile) {
            throw std::runtime_error(
                "Cannot create output.tac"
            );
        }

        CodeGenerator fileGenerator(tacFile);
        fileGenerator.generate(ast.get());
        tacFile.close();

        std::cout << "\nTAC saved to output.tac\n";

        // 3. Генерация графического представления AST
        std::ofstream dotFile("ast.dot");

        if (!dotFile) {
            throw std::runtime_error(
                "Cannot create ast.dot"
            );
        }

        ASTVisualizer visualizer(dotFile);
        visualizer.generate(ast.get());
        dotFile.close();

        std::cout << "AST graph saved to ast.dot\n";
    }
    catch (const std::exception& ex) {
        std::cerr << "\nError: " << ex.what() << '\n';
        return 1;
    }

    return 0;
}
