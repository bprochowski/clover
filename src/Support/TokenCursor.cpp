#include "TokenCursor.hpp"

#include <algorithm>

namespace {
const Token eof_token(Token::Type::eof);
}

TokenCursor::TokenCursor(std::span<Token const> const tokens)
    : tokens_(tokens)
    , position_(0)
{
}

Token TokenCursor::peek(uint8_t const offset)
{
    if (is_at_end_(offset)) {
        return eof_token;
    }

    return tokens_[position_ + offset];
}

Token TokenCursor::consume()
{
    if (is_at_end_()) {
        return eof_token;
    }

    return tokens_[position_++];
}

void TokenCursor::advance()
{
    if (!is_at_end_(1)) {
        ++position_;
    }
}

bool TokenCursor::match(Token::Type const token_type)
{
    if (peek().is_type(token_type)) {
        advance();
        return true;
    }

    return false;
}

bool TokenCursor::is_at_end_(uint8_t const offset) const
{
    return position_ + offset >= tokens_.size();
}
