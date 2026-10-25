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

    return std::move(fn.value().value());
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
        std::cout << "Missing return type initializer";
        exit(1);
    }

    auto return_type = tokens_.peek();
    if (!return_type.is_type(Token::Type::identifier)) {
        std::cout << "mising return type" << std::endl;
        exit(1);
    }
    tokens_.advance();

    if (!tokens_.match(Token::Type::left_brace)) {
        std::cout << "Missing function body opening '{'" << std::endl;
        exit(1);
    }

    FunctionBody fb;
    while (true) {
        auto stmt = statement_();
        if (!stmt.has_value()) {
            break;
        }

        fb.push_back(std::move(stmt.value()));
    }

    if (!tokens_.match(Token::Type::right_brace)) {
        std::cout << "Missing function body closing'}'" << std::endl;
        exit(1);
    }

    return ast::Function(token_name.literal().value(), return_type.literal().value(), std::move(fb));
}

OptionalStatement Parser::statement_()
{
    if (auto ret_stmt = return_stmt_(); ret_stmt.has_value()) {
        auto& ret = ret_stmt.value();
        if (ret.has_value()) {
            return std::move(ret.value());
        }

        std::cout << ret.error() << std::endl;
    }

    if (auto variable = variable_(); variable.has_value()) {
        auto& var = variable.value();
        if (var.has_value()) {
            return std::move(var.value());
        }

        std::cout << var.error() << std::endl;
    }

    if (auto if_stmt = if_(); if_stmt.has_value()) {
        auto& if_smt = if_stmt.value();
        if (if_smt.has_value()) {
            return std::move(if_smt.value());
        }

        std::cout << if_smt.error() << std::endl;
    }

    if (auto while_stmt = while_(); while_stmt.has_value()) {
        auto& ret = while_stmt.value();
        if (ret.has_value()) {
            return std::move(ret.value());
        }

        std::cout << ret.error() << std::endl;
    }

    return std::nullopt;
}

OptionalReturn Parser::return_stmt_()
{
    if (!tokens_.match(Token::Type::kw_return)) {
        return std::nullopt;
    }

    auto expr = expression_();

    if (!tokens_.match(Token::Type::semicolon)) {
        std::cout << tokens_.peek().literal().value();
        return std::unexpected("Missing termination");
    }

    return ast::Return(std::move(expr));
}

OptionalVariable Parser::variable_()
{
    if (!tokens_.match(Token::Type::kw_var)) {
        return std::nullopt;
    }

    auto identifier = tokens_.peek();
    if (!identifier.is_type(Token::Type::identifier)) {
        return std::unexpected("missing identifier name");
    }

    tokens_.advance();

    std::optional<std::string> type = std::nullopt;
    if (tokens_.match(Token::Type::arrow)) {
        auto type_token = tokens_.peek();
        if (!type_token.is_type(Token::Type::identifier)) {
            return std::unexpected("missing type");
        }

        type = type_token.literal();
        tokens_.advance();
    }

    std::unique_ptr<Expression> expr;
    if (tokens_.match(Token::Type::equal)) {
        expr = expression_();
    }

    if (!tokens_.match(Token::Type::semicolon)) {
        return std::unexpected("Missing termination");
    }

    return ast::Variable(identifier.literal().value(), type, std::move(expr));
}

OptionalIf Parser::if_()
{
    if (!tokens_.match(Token::Type::kw_if)) {
        return std::nullopt;
    }

    if (!tokens_.match(Token::Type::left_paren)) {
        return std::unexpected("Missing (");
    }

    auto expr = expression_();

    if (!tokens_.match(Token::Type::right_paren)) {
        return std::unexpected("Missing )");
    }

    FunctionBody then_block;
    if (!tokens_.match(Token::Type::left_brace)) {
        return std::unexpected("Missing {");
    }

    while (true) {
        auto stmt = statement_();
        if (!stmt.has_value()) {
            break;
        }

        then_block.push_back(std::move(stmt.value()));
    }

    if (!tokens_.match(Token::Type::right_brace)) {
        return std::unexpected("Missing }");
    }

    FunctionBody else_block;
    if (tokens_.match(Token::Type::kw_else)) {
        if (!tokens_.match(Token::Type::left_brace)) {
            return std::unexpected("Missing {");
        }

        while (true) {
            auto stmt = statement_();
            if (!stmt.has_value()) {
                break;
            }

            else_block.push_back(std::move(stmt.value()));
        }

        if (!tokens_.match(Token::Type::right_brace)) {
            return std::unexpected("Missing }");
        }
    }

    return ast::If(std::move(expr), std::move(then_block), std::move(else_block));
}

OptionalWhile Parser::while_()
{
    if (!tokens_.match(Token::Type::kw_while)) {
        return std::nullopt;
    }

    if (!tokens_.match(Token::Type::left_paren)) {
        return std::unexpected("Missing (");
    }

    auto expr = expression_();

    if (!tokens_.match(Token::Type::right_paren)) {
        return std::unexpected("Missing )");
    }

    FunctionBody body;
    if (!tokens_.match(Token::Type::left_brace)) {
        return std::unexpected("Missing {");
    }

    while (true) {
        auto stmt = statement_();
        if (!stmt.has_value()) {
            break;
        }

        body.push_back(std::move(stmt.value()));
    }

    if (!tokens_.match(Token::Type::right_brace)) {
        return std::unexpected("Missing }");
    }

    return ast::While(std::move(expr), std::move(body));
}

std::unique_ptr<Expression> Parser::expression_()
{
    return std::move(expression_with_precedence_(0));
}

std::unique_ptr<Expression> Parser::expression_with_precedence_(uint16_t min_precedence)
{
    auto lhs = primary_();

    while (true) {
        auto binary_operator = binary_operator_(tokens_.peek().type());
        if (!binary_operator.has_value()) {
            break;
        }

        auto const [op, lbp, rbp] = binary_operator.value();
        if (lbp < min_precedence) {
            break;
        }

        tokens_.advance();

        auto rhs = expression_with_precedence_(rbp);
        lhs = std::make_unique<Expression>(BinaryExpression(op, std::move(lhs), std::move(rhs)));
    }

    return lhs;
}

std::unique_ptr<Expression> Parser::primary_()
{
    if (tokens_.match(Token::Type::left_paren)) {
        auto lhs = expression_with_precedence_(0);

        if (!tokens_.match(Token::Type::right_paren)) {
            std::cout << "Unterminated paren expr";
            exit(1);
        }

        return lhs;
    }

    auto token = tokens_.peek();
    if (tokens_.match(Token::Type::identifier)) {
        return std::make_unique<Expression>(Identifier(token.literal().value()));
    }

    if (tokens_.match(Token::Type::number)) {
        return std::make_unique<Expression>(Integer(token.literal().value()));
    }

    std::cout << "Unexpected token for expr";
    exit(1);
}

std::optional<Operator> Parser::binary_operator_(Token::Type const type)
{
    switch (type) {
    case Token::Type::plus:
        return Operator(BinaryExpression::Operator::plus, 1, 2);
    case Token::Type::minus:
        return Operator(BinaryExpression::Operator::minus, 1, 2);
    case Token::Type::slash:
        return Operator(BinaryExpression::Operator::slash, 3, 4);
    case Token::Type::asterisk:
        return Operator(BinaryExpression::Operator::asterisk, 3, 4);
    default:
        return std::nullopt;
    }
}
