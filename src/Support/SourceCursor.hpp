#pragma once

#include <string_view>
#include <vector>

class SourceCursor {
public:
    SourceCursor(std::string_view const source);

    char peek(uint8_t const offset = 0);
    char consume();
    void advance();
    bool match(char const character);

    void skip_while(std::vector<char> const& char_to_skip);

private:
    bool is_at_end_(uint8_t const offset = 0) const;

    std::string_view const source_;
    uint16_t position_;
};