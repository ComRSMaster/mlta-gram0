#pragma once

#include <expected>
#include <istream>
#include <string>
#include <vector>

#include "rule.hpp"

struct parse_result {
    std::vector<rule> parsed_rules;
    std::vector<uint32_t> parsed_input_string;
    std::vector<std::string> token_names;
    int non_term_bal = 0;
};

[[nodiscard]] std::expected<parse_result, std::string> parse(
    std::istream& program_stream, const std::string& input_string);
