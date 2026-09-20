#include "Token.hpp"

Token::Token(Type const type)
    : type_(type)
{
}

bool Token::is_type(Type const type) const
{
    return type == type_;
}