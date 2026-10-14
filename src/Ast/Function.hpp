#pragma once

#include "Return.hpp"
#include "Variable.hpp"

#include <string>
#include <variant>
#include <vector>

using Statement = std::variant<ast::Variable, ast::Return>;
using FunctionBody = std::vector<Statement>;

namespace ast {
class Function {
public:
    explicit Function(std::string const& name, FunctionBody body);
    std::string name() const;
    FunctionBody const& body() const;

private:
    std::string name_;
    FunctionBody body_;
};
}