#pragma once

#include "Value.hpp"

namespace ir {
class Return {
public:
    Return(Value const& value)
        : value_(value)
    {
    }

    Value value() const
    {
        return value_;
    }

private:
    Value value_;
};
}