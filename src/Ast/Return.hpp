#pragma once

#include "Return.hpp"

#include "Ast/BinaryExpression.hpp"
#include "Ast/Identifier.hpp"
#include "Ast/Integer.hpp"

#include <string>

namespace ast {
class Return {
public:
    explicit Return(std::unique_ptr<Expression> expr);
    Expression const& value() const;

private:
    std::unique_ptr<Expression> expr_;
};
}