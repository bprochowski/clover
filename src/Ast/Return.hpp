#pragma once

#include "Return.hpp"

#include <string>

namespace ast {
class Return {
public:
    explicit Return(std::string const& value);
    std::string value() const;

private:
    std::string value_;
};
}