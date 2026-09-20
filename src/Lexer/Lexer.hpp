#pragma once

#include <expected>
#include <optional>
#include <vector>

#include "Lexer/Token.hpp"
#include "Support/Source.hpp"
#include "Support/SourceCursor.hpp"

using TokenOrError = std::expected<Token, std::string>;

class Lexer {
public:
    explicit Lexer(Source const& source);
    std::vector<Token> tokenize();

private:
    TokenOrError get_token_();
    std::optional<Token> identifier_or_keyword_();
    std::optional<Token> number_literal_();

    Source const& source_;
    SourceCursor cursor_;
};