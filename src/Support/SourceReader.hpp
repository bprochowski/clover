#pragma once

#include <expected>
#include <filesystem>
#include <string>
#include <system_error>

namespace source_reader {
using LoadResult = std::expected<std::string, std::string>;

LoadResult load(std::filesystem::path const& path);
};
