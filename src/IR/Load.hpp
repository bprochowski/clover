#pragma once

#include "Value.hpp"

namespace ir {
class Load {
public:
    Load(VirtualVar const& dest, std::string_view const name)
        : dest_(dest)
        , name_(name)
    {
    }

    VirtualVar get_destination() const
    {
        return dest_;
    }

    std::string get_name() const
    {
        return name_;
    }

private:
    VirtualVar dest_;
    std::string name_;
};
}