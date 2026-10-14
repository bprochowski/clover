#pragma once

#include <string>
#include <string_view>

#include "Value.hpp"

namespace ir {
class Store {
public:
    Store(std::string_view const name, Value const& value)
        : name_(name)
        , value_(value)
    {
    }
    std::string get_name() const { return name_; }
    Value value() const { return value_; }

private:
    std::string name_;
    Value value_;
};
}
