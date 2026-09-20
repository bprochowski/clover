#pragma once

#include <string>
#include <string_view>

class Source {
public:
    Source(std::string const& text);

    std::string_view text_view() const;
    std::string slice(uint16_t const begin, uint16_t const end) const;

private:
    std::string text_;
};