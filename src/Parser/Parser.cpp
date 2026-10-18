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

    auto variable = variable_();
    if (!variable.has_value()) {
        std::cout << variable.error() << std::endl;
        exit(1);
    }

    auto if_stmt = if_();
    if (!if_stmt.has_value()) {
        std::cout << if_stmt.error() << std::endl;
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

    FunctionBody fb;
    fb.push_back(std::move(*variable));
    fb.push_back(std::move(*if_stmt));
    fb.push_back(std::move(*return_stmt));
    return ast::Function(token_name.literal().value(), std::move(fb));
}

ReturnOrError Parser::return_stmt_()
{
    if (!tokens_.match(Token::Type::kw_return)) {
        return std::unexpected("Missing return keyword");
    }

    auto expr = expression_();

    if (!tokens_.match(Token::Type::semicolon)) {
        std::cout << tokens_.peek().literal().value();
        return std::unexpected("Missing termination");
    }

    return ast::Return(std::move(expr));
}

VariableOrError Parser::variable_()
{
    if (!tokens_.match(Token::Type::kw_var)) {
        return std::unexpected("missing kw var");
    }

    auto identifier = tokens_.peek();
    if (!identifier.is_type(Token::Type::identifier)) {
        return std::unexpected("missing identifier name");
    }

    tokens_.advance();

    std::unique_ptr<Expression> expr;
    if (tokens_.match(Token::Type::equal)) {
        expr = expression_();
    }

    if (!tokens_.match(Token::Type::semicolon)) {
        return std::unexpected("Missing termination");
    }

    return ast::Variable(identifier.literal().value(), std::move(expr));
}

IfOrError Parser::if_()
{
    if (!tokens_.match(Token::Type::kw_if)) {
        return std::unexpected("Missing kw if");
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

    auto variable = variable_();
    if (!variable.has_value()) {
        std::cout << variable.error() << std::endl;
        exit(1);
    }

    auto return_stmt = return_stmt_();
    if (!return_stmt.has_value()) {
        std::cout << return_stmt.error() << std::endl;
        exit(1);
    }

    if (!tokens_.match(Token::Type::right_brace)) {
        return std::unexpected("Missing }");
    }

    then_block.push_back(std::move(*variable));
    then_block.push_back(std::move(*return_stmt));

    FunctionBody else_block;
    if (tokens_.match(Token::Type::kw_else)) {
        if (!tokens_.match(Token::Type::left_brace)) {
            return std::unexpected("Missing {");
        }

        auto var_else = variable_();
        if (!var_else.has_value()) {
            std::cout << var_else.error() << std::endl;
            exit(1);
        }

        auto ret_else = return_stmt_();
        if (!ret_else.has_value()) {
            std::cout << ret_else.error() << std::endl;
            exit(1);
        }

        if (!tokens_.match(Token::Type::right_brace)) {
            return std::unexpected("Missing }");
        }

        else_block.push_back(std::move(*var_else));
        else_block.push_back(std::move(*ret_else));
    }

    return ast::If(std::move(expr), std::move(then_block), std::move(else_block));
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
