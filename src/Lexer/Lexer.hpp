#pragma once

#include "Support/SourceCursor.hpp"

class Lexer {
public:
    explicit Lexer(SourceCursor const& cursor);

private:
    SourceCursor const& cursor_;
};