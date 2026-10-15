#pragma once

#include "Return.hpp"
#include "Variable.hpp"

#include <variant>
#include <vector>

namespace ast {
class If;
}

using Statement = std::variant<ast::Variable, ast::Return, ast::If>;
using StatementList = std::vector<Statement>;
