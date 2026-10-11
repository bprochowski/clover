#pragma once

#include <string>

class Identifier {
public:
    Identifier(std::string const& name)
        : name_(name)
    {
    }

    std::string name() const
    {
        return name_;
    }

private:
    std::string name_;
};