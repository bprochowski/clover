#pragma once

#include <memory>
#include <variant>

#include "Identifier.hpp"
#include "Integer.hpp"

class BinaryExpression;

using Expression = std::variant<Identifier, Integer, BinaryExpression>;

class BinaryExpression {
public:
    enum class Operator {
        plus,
        minus,
        slash,
        asterisk
    };

    BinaryExpression(Operator op, std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs)
        : op_(op)
        , lhs_(std::move(lhs))
        , rhs_(std::move(rhs))
    {
    }

    Operator op() const
    {
        return op_;
    }

    Expression const& lhs() const
    {
        return *lhs_;
    }

    Expression const& rhs() const
    {
        return *rhs_;
    }

private:
    Operator op_;
    std::unique_ptr<Expression> lhs_;
    std::unique_ptr<Expression> rhs_;
};