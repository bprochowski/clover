#pragma once

#include "BinaryExpression.hpp"
#include "Statement.hpp"

namespace ast {
class If {
public:
    explicit If(std::unique_ptr<Expression> cond, StatementList then_block, StatementList else_block)
        : condition_(std::move(cond))
        , then_(std::move(then_block))
        , else_(std::move(else_block))
    {
    }

    Expression const& condition() const
    {
        return *condition_;
    }

    StatementList const& then_block() const
    {
        return then_;
    }

    StatementList const& else_block() const
    {
        return else_;
    }

private:
    std::unique_ptr<Expression> condition_;
    StatementList then_;
    StatementList else_;
};
}
