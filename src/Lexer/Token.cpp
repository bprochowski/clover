#include "Token.hpp"

Token::Token(Type const type, std::optional<std::string> value)
    : type_(type)
    , value_(value)
{
}

bool Token::is_type(Type const type) const
{
    return type == type_;
}

std::optional<std::string> Token::literal()
{
    return value_;
}