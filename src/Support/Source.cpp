#include "Source.hpp"

Source::Source(std::string const& text)
    : text_(text)
{
}

std::string_view Source::text_view() const
{
    return text_;
}

std::string Source::slice(uint16_t const begin, uint16_t const end) const
{
    uint16_t const length = end - begin;
    return text_.substr(begin, length);
}
