#pragma once

#include "Value.hpp"

namespace ir {
struct Branch {
    Value condition;
    std::string then_;
    std::string else_;
};
}