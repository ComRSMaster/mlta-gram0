#pragma once

#include <expected>
#include <string>

#include "parser.hpp"

[[nodiscard]] std::expected<std::string, std::string> run(
    const parse_result& parsed_data, bool is_verbose);
