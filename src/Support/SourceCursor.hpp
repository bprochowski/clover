#pragma once

#include <string_view>

class SourceCursor {
public:
    SourceCursor(std::string_view const source);

private:
    std::string_view const source_;
};