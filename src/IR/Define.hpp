#pragma once

#include <string>
#include <string_view>

namespace ir {
class Define {
public:
    explicit Define(std::string_view const name)
        : name_(name)
    {
    }
    std::string get_name() const { return name_; }

private:
    std::string name_;
};
}