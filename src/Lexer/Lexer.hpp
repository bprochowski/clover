#pragma once

#include <expected>
#include <optional>
#include <vector>

#include "Lexer/Token.hpp"
#include "Support/Source.hpp"
#include "Support/SourceCursor.hpp"

using TokenOrError = std::expected<Token, std::string>;
using Tokens = std::vector<Token>;
using Errors = std::vector<std::string>;
using TokenizationResult = std::expected<Tokens, Errors>;

class Lexer {
public:
    explicit Lexer(Source const& source);
    TokenizationResult tokenize();

private:
    TokenOrError get_token_();
    std::optional<Token> identifier_or_keyword_();
    std::optional<Token> number_literal_();

    Source const& source_;
    SourceCursor cursor_;
};