#pragma once

#include <string>
#include <vector>

#include "rule.hpp"

[[nodiscard]] int run(const std::vector<rule>& parsed_rules, uint32_t non_term_bal, std::vector<uint32_t>& current, std::string& result_string, const std::vector<std::string>& token_names, bool is_verbose);
