#include "parser.hpp"

#include <algorithm>
#include <cstdint>
#include <expected>
#include <sstream>
#include <string>
#include <vector>

#include "rule.hpp"

[[nodiscard]] std::expected<parse_result, std::string> parse(
    std::istream& program_stream, const std::string& input_string) {
    parse_result result;
    using full_rule_type =
        std::pair<std::vector<std::string>, std::vector<std::string>>;
    std::vector<full_rule_type> full_rules;

    std::string line;
    int line_number = 0;
    while (std::getline(program_stream, line)) {
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
                    return std::unexpected(std::format(
                        "Too many '->' in one line, only one allowed\nLine {}: "
                        "{}",
                        line_number, ss.str()));
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
            result.token_names.push_back(std::move(token));
        }
        if (!right_side) {
            return std::unexpected(std::format(
                "No '->' found in line\nLine {}: {}", line_number, ss.str()));
        }
        if (!has_non_term) {
            return std::unexpected(std::format(
                "Rule must replace at least one non-terminal symbol\nLine {}: {}",
                line_number, ss.str()));
        }
        full_rules.push_back(std::move(node));
    }

    std::stringstream ss(".B " + input_string + " .E");
    std::string token;
    while (ss >> token) {
        result.token_names.push_back(std::move(token));
    }

    result.token_names.push_back(".b");
    result.token_names.push_back(".e");

    std::sort(result.token_names.begin(), result.token_names.end());
    result.token_names.erase(
        std::unique(result.token_names.begin(), result.token_names.end()),
        result.token_names.end());

    result.parsed_rules.reserve(full_rules.size());
    for (const auto& [left, right] : full_rules) {
        rule node;

        node.left.reserve(left.size());
        for (const auto& str : left) {
            uint32_t idx = std::lower_bound(result.token_names.begin(),
                                            result.token_names.end(), str) -
                           result.token_names.begin();
            node.left.push_back(idx);
            if (str.starts_with('.')) {
                --node.non_term_diff;
            }
        }

        node.right.reserve(right.size());
        for (const auto& str : right) {
            uint32_t idx = std::lower_bound(result.token_names.begin(),
                                            result.token_names.end(), str) -
                           result.token_names.begin();
            node.right.push_back(idx);
            if (str.starts_with('.')) {
                ++node.non_term_diff;
            }
        }

        result.parsed_rules.push_back(std::move(node));
    }

    ss.clear();
    ss.seekg(0);
    while (ss >> token) {
        if (token.starts_with('.')) {
            ++result.non_term_bal;
        }
        uint32_t idx = std::lower_bound(result.token_names.begin(),
                                        result.token_names.end(), token) -
                       result.token_names.begin();
        result.parsed_input_string.push_back(idx);
    }

    return result;
}
