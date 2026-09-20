#include "Lexer.hpp"

Lexer::Lexer(Source const& source)
    : source_(source)
    , cursor_(source.text_view())
{
}