#pragma once

#include "If.hpp"
#include "Statement.hpp"
#include "While.hpp"

#include <string>

using FunctionBody = StatementList;

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