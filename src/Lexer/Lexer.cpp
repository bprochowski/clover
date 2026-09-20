#include "Lexer.hpp"

#include <cctype>
#include <format>
#include <iostream>

namespace {
const std::vector<char> whitespace_chars = { '\n', '\0', '\t', '\r', ' ' };

bool is_alpha(char const character)
{
    return std::isalpha(static_cast<unsigned char>(character));
}

bool is_digit(char const character)
{
    return std::isdigit(static_cast<unsigned char>(character));
}

bool is_identifier_start(char const character)
{
    return is_alpha(character) || character == '_';
}

bool is_identifier_continue(char const character)
{
    return is_identifier_start(character) || is_digit(character) || character == '-';
}
}

Lexer::Lexer(Source const& source)
    : source_(source)
    , cursor_(source.text_view())
{
}

std::vector<Token> Lexer::tokenize()
{
    std::vector<Token> tokens;

    while (true) {
        auto const token = get_token_();
        if (!token.has_value()) {
            std::cout << token.error() << std::endl;
            exit(1);
        }

        tokens.push_back(token.value());
        if (token->is_type(Token::Type::eof)) {
            break;
        }
    }

    return tokens;
}

TokenOrError Lexer::get_token_()
{
    cursor_.skip_while(whitespace_chars);

    if (auto const token = identifier_or_keyword_(); token.has_value()) {
        return token.value();
    }

    if (auto const token = number_literal_(); token.has_value()) {
        return token.value();
    }

    auto const consumed_character = cursor_.consume();
    switch (consumed_character) {
    case '\0': {
        return Token(Token::Type::eof);
    }
    case '(': {
        return Token(Token::Type::left_paren);
    }
    case ')': {
        return Token(Token::Type::right_paren);
    }
    case '{': {
        return Token(Token::Type::left_brace);
    }
    case '}': {
        return Token(Token::Type::right_brace);
    }
    case ';': {
        return Token(Token::Type::semicolon);
    }
    case '-': {
        if (cursor_.match('>')) {
            return Token(Token::Type::arrow);
        }

        return Token(Token::Type::minus);
    }
    default:
        break;
    }

    return std::unexpected(std::format("Unknown character: {}", consumed_character));
}

std::optional<Token> Lexer::identifier_or_keyword_()
{
    if (!is_identifier_start(cursor_.peek())) {
        return std::nullopt;
    }

    do {
        cursor_.advance();
    } while (is_identifier_continue(cursor_.peek()));

    return Token(Token::Type::identifier);
}

std::optional<Token> Lexer::number_literal_()
{
    if (!is_digit(cursor_.peek())) {
        return std::nullopt;
    }

    do {
        cursor_.advance();
    } while (is_digit(cursor_.peek()));

    return Token(Token::Type::number);
}