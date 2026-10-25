#pragma once

#include "If.hpp"
#include "Statement.hpp"
#include "While.hpp"

#include <string>

using FunctionBody = StatementList;

namespace ast {
class Function {
public:
    explicit Function(std::string const& name, std::string const& ret_type, FunctionBody body);
    std::string name() const;
    FunctionBody const& body() const;
    std::string return_type() const;

private:
    std::string name_;
    std::string return_type_;
    FunctionBody body_;
};
}