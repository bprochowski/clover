#pragma once

#include <optional>

#include "BinaryExpression.hpp"

namespace ast {
class Variable {
public:
    Variable(std::string_view const name, std::optional<std::string> const& type, std::unique_ptr<Expression> init)
        : name_(name)
        , type_(type)
        , init_(std::move(init))
    {
    }

    std::string name() const
    {
        return name_;
    }

    std::optional<std::string> declared_type() const
    {
        return type_;
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
    std::optional<std::string> type_;
    std::unique_ptr<Expression> init_;
};
}