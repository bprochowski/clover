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
    uint16_t position_ahead = position_ + offset;
    if (position_ahead < source_.length()) {
        return source_.at(position_ahead);
    }

    return source_end_mark;
}

char SourceCursor::advance()
{
    if (position_ + 1 < source_.length()) {
        return source_.at(position_++);
    }

    return source_end_mark;
}

void SourceCursor::skip_while(std::vector<char> const& char_to_skip)
{
    for (; position_ < source_.length(); ++position_) {
        if (std::find(char_to_skip.begin(), char_to_skip.end(), source_[position_]) == char_to_skip.end()) {
            break;
        }
    }
}