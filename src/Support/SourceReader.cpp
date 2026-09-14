#include "SourceReader.hpp"

#include <cerrno>
#include <format>
#include <fstream>
#include <sstream>

namespace {
constexpr std::string_view cannot_open_source_error = "Cannot open source file {}";
constexpr std::string_view cannot_read_source_error = "Cannot read source file {}";
}

namespace source_reader {
LoadResult load(std::filesystem::path const& path)
{
    std::ifstream source_file(path);
    auto const filename = path.filename().string();

    if (!source_file.is_open()) {
        return std::unexpected(std::format(cannot_open_source_error, filename));
    }

    std::stringstream source;
    source << source_file.rdbuf();
    if (source_file.bad()) {
        return std::unexpected(std::format(cannot_read_source_error, filename));
    }

    source << '\0';
    if (source.bad()) {
        return std::unexpected(std::format(cannot_read_source_error, filename));
    }

    return source.str();
}
}
