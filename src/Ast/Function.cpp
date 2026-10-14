#include "Function.hpp"

#include <utility>

namespace ast {
Function::Function(std::string const& name, FunctionBody body)
    : name_(name)
    , body_(std::move(body))
{
}

std::string Function::name() const
{
    return name_;
}

FunctionBody const& Function::body() const
{
    return body_;
}

}