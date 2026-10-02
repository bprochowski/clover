#pragma once

#include "Return.hpp"

#include <string>

namespace ast {
class Function {
public:
    explicit Function(std::string const& name, ast::Return body);
    std::string name() const;
    ast::Return body() const;

private:
    std::string name_;
    ast::Return body_;
};
}