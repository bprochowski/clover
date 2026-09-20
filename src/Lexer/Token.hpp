#pragma once

#include <stdint.h>

class Token {
public:
    enum class Type : uint16_t {
        eof = 0,
        number,
        identifier,
        left_paren,
        right_paren,
        left_brace,
        right_brace,
        semicolon,
        minus,
        arrow
    };

    Token(Type const type);
    bool is_type(Type const type) const;

private:
    Type type_;
};