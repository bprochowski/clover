#pragma once

#include "Return.hpp"
#include "Variable.hpp"

#include <variant>
#include <vector>

namespace ast {
class If;
class While;
}

using Statement = std::variant<ast::Variable, ast::Return, ast::If, ast::While>;
using StatementList = std::vector<Statement>;
