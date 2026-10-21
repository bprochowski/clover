#include "If.hpp"
#include "While.hpp"

#include <utility>

namespace ast {
If::If(std::unique_ptr<Expression> cond, StatementList then_block, StatementList else_block)
    : condition_(std::move(cond))
    , then_(std::move(then_block))
    , else_(std::move(else_block))
{
}

}
