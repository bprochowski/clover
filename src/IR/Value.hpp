#pragma once

#include <string>
#include <variant>

namespace ir {
struct VirtualVar {
    std::string id;
};

struct Constant {
    std::string value;
};

using Value = std::variant<Constant, VirtualVar>;
}