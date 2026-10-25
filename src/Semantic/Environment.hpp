#pragma once

#include <optional>
#include <ranges>
#include <string>
#include <unordered_map>
#include <vector>

#include "Type.hpp"

struct SymbolCtx {
    enum class Kind {
        variable,
        type
    };

    Kind kind;
    Type type;
};

class Environment {
public:
    Environment()
    {
        scopes_.emplace_back();
        scopes_.back().emplace("i32", SymbolCtx { SymbolCtx::Kind::type, Type::integer });
        scopes_.back().emplace("bool", SymbolCtx { SymbolCtx::Kind::type, Type::boolean });
    }

    void open_scope()
    {
        scopes_.emplace_back();
    }

    void close_scope()
    {
        scopes_.pop_back();
    }

    std::optional<SymbolCtx> lookup(std::string const& name)
    {
        for (auto const& scope : scopes_ | std::views::reverse) {
            if (auto it = scope.find(name); it != scope.end()) {
                return it->second;
            }
        }
        return std::nullopt;
    }

    bool declare(std::string const& name, SymbolCtx const symbol)
    {
        auto result = lookup(name);
        if (result.has_value()) {
            return false;
        }

        scopes_.back().try_emplace(name, symbol);
        return true;
    }

private:
    std::vector<std::unordered_map<std::string, SymbolCtx>> scopes_;
};