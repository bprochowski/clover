#include "Function.hpp"

namespace ast {
Function::Function(std::string const& name, ast::Return body)
    : name_(name)
    , body_(std::move(body))
{
}

std::string Function::name() const
{
    return name_;
}

ast::Return const& Function::body() const
{
    return body_;
}

}