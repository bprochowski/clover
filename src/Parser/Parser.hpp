#pragma once

#include "Ast/Function.hpp"
#include "Ast/Return.hpp"

#include "Ast/BinaryExpression.hpp"
#include "Ast/Identifier.hpp"
#include "Ast/If.hpp"
#include "Ast/Integer.hpp"
#include "Ast/Variable.hpp"

#include "Lexer/Token.hpp"
#include "Support/TokenCursor.hpp"

#include <expected>
#include <optional>
#include <utility>
#include <vector>

using FunctionOrError = std::expected<ast::Function, std::string>;
using ReturnOrError = std::expected<ast::Return, std::string>;
using VariableOrError = std::expected<ast::Variable, std::string>;
using IfOrError = std::expected<ast::If, std::string>;

struct Operator {
    BinaryExpression::Operator op;
    uint8_t lbp;
    uint8_t rbp;
};

class Parser {
public:
    explicit Parser(std::vector<Token> const& tokens);
    ast::Function build_ast();

private:
    std::optional<FunctionOrError> function_();
    ReturnOrError return_stmt_();
    VariableOrError variable_();
    IfOrError if_();
    std::unique_ptr<Expression> expression_();
    std::unique_ptr<Expression> expression_with_precedence_(uint16_t min_precedence);
    std::unique_ptr<Expression> primary_();
    std::optional<Operator> binary_operator_(Token::Type const type);

    TokenCursor tokens_;
};