#include "Return.hpp"

namespace ast {
Return::Return(std::string const& value)
    : value_(value)
{
}

std::string Return::value() const
{
    return value_;
}
}