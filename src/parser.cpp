#include "parser.hpp"

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "debug_helpers.hpp"
#include "rule.hpp"

[[nodiscard]] int parse(std::vector<rule>& parsed_rules, int& non_term_bal,
                        const std::string& input_string, std::vector<uint32_t>& parsed_input_string,
                        std::vector<std::string>& unique_tokens, std::ifstream& file) {
    using full_rule_type = std::pair<std::vector<std::string>, std::vector<std::string>>;
    std::vector<full_rule_type> full_rules;

    std::string line;
    int line_number = 0;
    while (std::getline(file, line)) {
        ++line_number;
        size_t first_idx = line.find_first_not_of(" \t\r\n");
        if (first_idx == std::string::npos || line[first_idx] == '#') {
            continue;
        }

        std::stringstream ss(std::move(line));
        std::string token;
        bool right_side = false;
        bool has_non_term = false;
        full_rule_type node;
        while (ss >> token) {
            if (token == "->") {
                if (right_side) {
                    return print_error("Too many '->' in one line, only one allowed\nLine {}: {}", line_number, ss.str());
                }
                right_side = true;
                continue;
            }
            if (right_side) {
                node.second.push_back(token);
            } else {
                if (token.starts_with('.')) {
                    has_non_term = true;
                }
                node.first.push_back(token);
            }
            unique_tokens.push_back(std::move(token));
        }
        if (!right_side) {
            return print_error("No '->' found in line\nLine {}: {}", line_number, ss.str());
        }
        if (!has_non_term) {
            return print_error("Rule must replace at least one symbol\nLine {}: {}", line_number, ss.str());
        }
        full_rules.push_back(std::move(node));
    }

    std::stringstream ss(".B " + input_string + " .E");
    std::string token;
    while (ss >> token) {
        unique_tokens.push_back(std::move(token));
    }

    unique_tokens.push_back(".b");
    unique_tokens.push_back(".e");

    std::sort(unique_tokens.begin(), unique_tokens.end());
    unique_tokens.erase(std::unique(unique_tokens.begin(), unique_tokens.end()), unique_tokens.end());

    parsed_rules.reserve(full_rules.size());
    for (const auto& [left, right] : full_rules) {
        rule node;

        node.left.reserve(left.size());
        for (const auto& str : left) {
            uint32_t idx = std::lower_bound(unique_tokens.begin(), unique_tokens.end(), str) - unique_tokens.begin();
            node.left.push_back(idx);
            if (str.starts_with('.')) {
                --node.non_term_diff;
            }
        }

        node.right.reserve(right.size());
        for (const auto& str : right) {
            uint32_t idx = std::lower_bound(unique_tokens.begin(), unique_tokens.end(), str) - unique_tokens.begin();
            node.right.push_back(idx);
            if (str.starts_with('.')) {
                ++node.non_term_diff;
            }
        }

        parsed_rules.push_back(std::move(node));
    }

    ss.clear();
    ss.seekg(0);
    while (ss >> token) {
        if (token.starts_with('.')) {
            ++non_term_bal;
        }
        uint32_t idx = std::lower_bound(unique_tokens.begin(), unique_tokens.end(), token) - unique_tokens.begin();
        parsed_input_string.push_back(idx);
    }

    return 0;
}
