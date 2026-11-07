
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wno-unused-function

TARGET = minicparser

SOURCES = parser.tab.c lex.yy.c main.cpp interpreter.cpp codegen.cpp ast_visualizer.cpp

.PHONY: all run test clean

all: $(TARGET)

parser.tab.c parser.tab.h &: parser.y ast.h
	bison -d -v parser.y

lex.yy.c: lexer.l parser.tab.h
	flex -o lex.yy.c lexer.l

$(TARGET): $(SOURCES) ast.h interpreter.h codegen.h ast_visualizer.h
	$(CXX) $(CXXFLAGS) -x c++ $(SOURCES) -o $(TARGET)

run: $(TARGET)
	./$(TARGET) examples/valid.c
	dot -Tsvg ast.dot -o ast.svg

test: $(TARGET)
	@echo "==================================="
	@echo "MiniCParser - Automated Tests"
	@echo "==================================="

	@echo "[1/4] Valid program"
	@./$(TARGET) examples/valid.c > /dev/null && echo "PASS: Valid program accepted"

	@echo "[2/4] Syntax error"
	@if ./$(TARGET) examples/invalid.c > /dev/null 2>&1; then \
		echo "FAIL: Invalid syntax accepted"; \
		exit 1; \
	else \
		echo "PASS: Syntax error detected"; \
	fi

	@echo "[3/4] Division by zero"
	@if ./$(TARGET) examples/division_by_zero.c > /dev/null 2>&1; then \
		echo "FAIL: Division by zero not detected"; \
		exit 1; \
	else \
		echo "PASS: Division by zero detected"; \
	fi

	@echo "[4/4] Undefined variable"
	@if ./$(TARGET) examples/undefined_variable.c > /dev/null 2>&1; then \
		echo "FAIL: Undefined variable not detected"; \
		exit 1; \
	else \
		echo "PASS: Undefined variable detected"; \
	fi

	@echo "==================================="
	@echo "All 4 tests completed successfully!"
	@echo "==================================="

clean:
	rm -f $(TARGET) parser.tab.c parser.tab.h parser.output lex.yy.c output.tac ast.dot ast.svg
