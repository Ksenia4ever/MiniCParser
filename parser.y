%{
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>
#include "ast.h"

extern int yylex();
extern int yylineno;

void yyerror(const char* message);

ASTNode* root = nullptr;

static ASTNode* makeNode(NodeType type, const std::string& value = "") {
    return new ASTNode(type, value);
}

static void addChild(ASTNode* parent, ASTNode* child) {
    parent->addChild(std::unique_ptr<ASTNode>(child));
}
%}

%code requires {
    struct ASTNode;
}

%union {
    double number;
    char* text;
    ASTNode* node;
}

%token <number> NUMBER
%token <text> IDENTIFIER
%token INT PRINT
%token ASSIGN PLUS MINUS MULTIPLY DIVIDE
%token LPAREN RPAREN SEMICOLON

%type <node> program statements statement expression

%left PLUS MINUS
%left MULTIPLY DIVIDE
%right UMINUS

%start program

%%

program:
    statements {
        root = $1;
        $$ = $1;
    }
    ;

statements:
    statement {
        $$ = makeNode(NodeType::Block);
        addChild($$, $1);
    }
    | statements statement {
        $$ = $1;
        addChild($$, $2);
    }
    ;

statement:
    INT IDENTIFIER SEMICOLON {
        $$ = makeNode(NodeType::Declaration, $2);
        free($2);
    }
    | INT IDENTIFIER ASSIGN expression SEMICOLON {
        $$ = makeNode(NodeType::Declaration, $2);
        addChild($$, $4);
        free($2);
    }
    | IDENTIFIER ASSIGN expression SEMICOLON {
        $$ = makeNode(NodeType::Assignment, $1);
        addChild($$, $3);
        free($1);
    }
    | PRINT LPAREN expression RPAREN SEMICOLON {
        $$ = makeNode(NodeType::Print);
        addChild($$, $3);
    }
    ;

expression:
    NUMBER {
        $$ = makeNode(NodeType::Number, std::to_string($1));
    }
    | IDENTIFIER {
        $$ = makeNode(NodeType::Variable, $1);
        free($1);
    }
    | expression PLUS expression {
        $$ = makeNode(NodeType::BinaryOperation, "+");
        addChild($$, $1);
        addChild($$, $3);
    }
    | expression MINUS expression {
        $$ = makeNode(NodeType::BinaryOperation, "-");
        addChild($$, $1);
        addChild($$, $3);
    }
    | expression MULTIPLY expression {
        $$ = makeNode(NodeType::BinaryOperation, "*");
        addChild($$, $1);
        addChild($$, $3);
    }
    | expression DIVIDE expression {
        $$ = makeNode(NodeType::BinaryOperation, "/");
        addChild($$, $1);
        addChild($$, $3);
    }
    | MINUS expression %prec UMINUS {
        $$ = makeNode(NodeType::UnaryOperation, "-");
        addChild($$, $2);
    }
    | LPAREN expression RPAREN {
        $$ = $2;
    }
    ;

%%

void yyerror(const char* message) {
    fprintf(stderr, "Syntax error at line %d: %s\n", yylineno, message);
}