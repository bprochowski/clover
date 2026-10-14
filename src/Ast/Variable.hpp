#pragma once

#include "BinaryExpression.hpp"

namespace ast {
class Variable {
public:
    Variable(std::string_view const name, std::unique_ptr<Expression> init)
        : name_(name)
        , init_(std::move(init))
    {
    }

    std::string name() const
    {
        return name_;
    }

    bool has_initializer() const
    {
        return init_ != nullptr;
    }

    Expression const& initializer() const
    {
        return *init_;
    }

private:
    std::string name_;
    std::unique_ptr<Expression> init_;
};
}