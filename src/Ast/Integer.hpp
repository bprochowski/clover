#pragma once

#include <string>

class Integer {
public:
    Integer(std::string const& value)
        : value_(value)
    {
    }

    std::string value() const
    {
        return value_;
    }

private:
    std::string value_;
};