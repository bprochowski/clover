#include "While.hpp"
#include "If.hpp"

#include <utility>

namespace ast {
While::While(std::unique_ptr<Expression> cond, StatementList body)
    : condition_(std::move(cond))
    , body_(std::move(body))
{
}

}
