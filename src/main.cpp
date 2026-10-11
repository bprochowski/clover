#include <fstream>
#include <iostream>

#include "Support/Source.hpp"
#include "Support/SourceCursor.hpp"
#include "Support/SourceReader.hpp"

#include "Lexer/Lexer.hpp"
#include "Parser/Parser.hpp"

namespace {
char const* operator_symbol(BinaryExpression::Operator const op)
{
    switch (op) {
    case BinaryExpression::Operator::plus:
        return "+";
    case BinaryExpression::Operator::minus:
        return "-";
    case BinaryExpression::Operator::slash:
        return "/";
    case BinaryExpression::Operator::asterisk:
        return "*";
    }

    return "?";
}

struct ExpressionPrinter {
    int depth = 0;

    void operator()(Identifier const& node) const
    {
        std::cout << indent() << "Identifier(" << node.name() << ")" << std::endl;
    }

    void operator()(Integer const& node) const
    {
        std::cout << indent() << "Integer(" << node.value() << ")" << std::endl;
    }

    void operator()(BinaryExpression const& node) const
    {
        std::cout << indent() << "BinaryExpression(" << operator_symbol(node.op()) << ")" << std::endl;
        std::visit(ExpressionPrinter { depth + 1 }, node.lhs());
        std::visit(ExpressionPrinter { depth + 1 }, node.rhs());
    }

    std::string indent() const
    {
        return std::string(depth * 2, ' ');
    }
};
}

int main(int argc, char** argv)
{
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " [source-file]" << std::endl;
        return 1;
    }

    auto const source_text = source_reader::load(argv[1]);
    if (!source_text.has_value()) {
        std::cout << source_text.error() << std::endl;
        return 1;
    }

    Source source(source_text.value());
    Lexer lexer(source);
    auto tokens = lexer.tokenize();
    if (!tokens.has_value()) {
        for (auto const& error : tokens.error()) {
            std::cout << error << std::endl;
        }

        exit(1);
    }

    Parser parser(tokens.value());
    auto fn_node = parser.build_ast();

    std::visit(ExpressionPrinter { }, fn_node.body().value());

    return 0;
}