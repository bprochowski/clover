#pragma once

#include "Support/Source.hpp"
#include "Support/SourceCursor.hpp"

class Lexer {
public:
    explicit Lexer(Source const& source);

private:
    Source const& source_;
    SourceCursor cursor_;
};