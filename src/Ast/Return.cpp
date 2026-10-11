#include "Return.hpp"

namespace ast {
Return::Return(std::unique_ptr<Expression> expr)
    : expr_(std::move(expr))
{
}

Expression const& Return::value() const
{
    return *expr_;
}
}