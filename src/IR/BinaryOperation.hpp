#pragma once

#include "Value.hpp"

namespace ir {
class BinaryOperation {
public:
    enum class Kind {
        add,
        sub,
        mul,
        div
    };

    BinaryOperation(Kind kind, VirtualVar const& dest, Value const& lhs, Value const& rhs)
        : kind_(kind)
        , dest_(dest)
        , lhs_(lhs)
        , rhs_(rhs)
    {
    }

    Kind get_kind() const
    {
        return kind_;
    }

    VirtualVar get_destination() const
    {
        return dest_;
    }

    Value lhs() const
    {
        return lhs_;
    }

    Value rhs() const
    {
        return rhs_;
    }

private:
    Kind kind_;
    VirtualVar dest_;
    Value lhs_;
    Value rhs_;
};
}