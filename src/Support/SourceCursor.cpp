#include "SourceCursor.hpp"

#include <algorithm>

namespace {
constexpr char source_end_mark = '\0';
}

SourceCursor::SourceCursor(std::string_view const source)
    : source_(source)
    , position_(0)
{
}

char SourceCursor::peek(uint8_t const offset)
{
    if (is_at_end_(offset)) {
        return source_end_mark;
    }

    return source_[position_ + offset];
}

char SourceCursor::consume()
{
    if (is_at_end_()) {
        return source_end_mark;
    }

    return source_[position_++];
}

void SourceCursor::advance()
{
    if (!is_at_end_(1)) {
        ++position_;
    }
}

bool SourceCursor::match(char const character)
{
    if (peek() == character) {
        advance();
        return true;
    }

    return false;
}

void SourceCursor::skip_while(std::vector<char> const& char_to_skip)
{
    while (!is_at_end_()) {
        if (std::find(char_to_skip.begin(), char_to_skip.end(), source_[position_]) == char_to_skip.end()) {
            break;
        }

        ++position_;
    }
}

bool SourceCursor::is_at_end_(uint8_t const offset) const
{
    return position_ + offset >= source_.length();
}
