#pragma once

#include "Ast/Function.hpp"
#include "Ast/Return.hpp"

#include "Lexer/Token.hpp"
#include "Support/TokenCursor.hpp"

#include <expected>
#include <optional>
#include <vector>

using FunctionOrError = std::expected<ast::Function, std::string>;
using ReturnOrError = std::expected<ast::Return, std::string>;

class Parser {
public:
    explicit Parser(std::vector<Token> const& tokens);
    ast::Function build_ast();

private:
    std::optional<FunctionOrError> function_();
    ReturnOrError return_stmt_();
    TokenCursor tokens_;
};