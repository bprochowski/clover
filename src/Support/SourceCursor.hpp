#pragma once

#include <string_view>
#include <vector>

class SourceCursor {
public:
    SourceCursor(std::string_view const source);

    char peek(uint8_t const offset);
    char advance();
    void skip_while(std::vector<char> const& char_to_skip);

private:
    std::string_view const source_;
    uint16_t position_;
};