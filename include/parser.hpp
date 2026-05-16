#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "rule.hpp"

[[nodiscard]] int parse(std::vector<rule>& parsed_rules, int& non_term_bal,
                        const std::string& input_string, std::vector<uint32_t>& parsed_input_string,
                        std::vector<std::string>& unique_tokens, std::ifstream& file);
