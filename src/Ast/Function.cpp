#include "Function.hpp"

#include <utility>

namespace ast {
Function::Function(std::string const& name, std::string const& ret_type, FunctionBody body)
    : name_(name)
    , return_type_(ret_type)
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

std::string Function::return_type() const
{
    return return_type_;
}

}