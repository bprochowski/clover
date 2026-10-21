#pragma once

#include "BinaryExpression.hpp"
#include "Statement.hpp"

namespace ast {
class While {
public:
    explicit While(std::unique_ptr<Expression> cond, StatementList body);

    Expression const& condition() const
    {
        return *condition_;
    }

    StatementList const& body() const
    {
        return body_;
    }

private:
    std::unique_ptr<Expression> condition_;
    StatementList body_;
};
}
