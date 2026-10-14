#pragma once

#include <optional>
#include <stdint.h>
#include <string>

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
        plus,
        slash,
        asterisk,
        arrow,
        equal,
        kw_fn,
        kw_var
    };

    Token(Type const type, std::optional<std::string> value = std::nullopt);
    bool is_type(Type const type) const;
    Token::Type type() const;
    std::optional<std::string> literal();

private:
    Type type_;
    std::optional<std::string> value_;
};