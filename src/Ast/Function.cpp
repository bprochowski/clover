#include "Function.hpp"

namespace ast {
Function::Function(std::string const& name, ast::Return body)
    : name_(name)
    , body_(body)
{
}

std::string Function::name() const
{
    return name_;
}

ast::Return Function::body() const
{
    return body_;
}

}