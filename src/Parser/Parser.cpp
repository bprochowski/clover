#include "Parser.hpp"

#include <iostream>

Parser::Parser(std::vector<Token> const& tokens)
    : tokens_(tokens)
{
}

ast::Function Parser::build_ast()
{
    auto fn = function_();
    if (!fn.has_value()) {
        std::cout << "Unexpected token.." << std::endl;
        exit(1);
    }

    return fn.value().value();
}

std::optional<FunctionOrError> Parser::function_()
{
    if (!tokens_.match(Token::Type::kw_fn)) {
        return std::nullopt;
    }

    auto token_name = tokens_.peek();
    if (!token_name.is_type(Token::Type::identifier)) {
        std::cout << "Missing function name" << std::endl;
        exit(1);
    }

    tokens_.advance();

    if (!tokens_.match(Token::Type::left_paren)) {
        std::cout << "Missing arguement openning '{'" << std::endl;
        exit(1);
    }

    if (!tokens_.match(Token::Type::right_paren)) {
        std::cout << "Missing arguement closing '}'" << std::endl;
        exit(1);
    }

    if (!tokens_.match(Token::Type::arrow)) {
        std::cout << "Missing return type" << std::endl;
        exit(1);
    }

    if (!tokens_.match(Token::Type::identifier)) {
        std::cout << "Missing return type identifier" << std::endl;
        exit(1);
    }

    if (!tokens_.match(Token::Type::left_brace)) {
        std::cout << "Missing function body opening '{'" << std::endl;
        exit(1);
    }

    auto return_stmt = return_stmt_();
    if (!return_stmt.has_value()) {
        std::cout << return_stmt.error() << std::endl;
        exit(1);
    }

    if (!tokens_.match(Token::Type::right_brace)) {
        std::cout << "Missing function body closing'}'" << std::endl;
        exit(1);
    }

    return ast::Function(token_name.literal().value(), return_stmt.value());
}

ReturnOrError Parser::return_stmt_()
{
    if (!tokens_.match(Token::Type::identifier)) {
        return std::unexpected("Missing return keyword");
    }

    auto value_token = tokens_.peek();
    if (!value_token.is_type(Token::Type::number)) {
        return std::unexpected("Missing number");
    }

    tokens_.advance();

    if (!tokens_.match(Token::Type::semicolon)) {
        return std::unexpected("Missing termination");
    }

    return ast::Return(value_token.literal().value());
}