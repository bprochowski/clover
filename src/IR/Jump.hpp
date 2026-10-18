#pragma once

#include <string>
#include <string_view>

namespace ir {
class Jump {
public:
    Jump(std::string_view const lable)
        : lable_(lable)
    {
    }

    std::string lable() const
    {
        return lable_;
    }

private:
    std::string lable_;
};
}