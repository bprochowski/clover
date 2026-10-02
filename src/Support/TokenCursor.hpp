#pragma once

#include "Lexer/Token.hpp"

#include <span>
#include <vector>

class TokenCursor {
public:
    TokenCursor(std::span<Token const> const tokens);

    Token peek(uint8_t const offset = 0);
    Token consume();
    void advance();
    bool match(Token::Type type);

private:
    bool is_at_end_(uint8_t const offset = 0) const;

    std::span<Token const> const tokens_;
    uint16_t position_;
};