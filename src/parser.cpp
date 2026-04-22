#include <algorithm>
#include <cstdint>
#include <fstream>
#include <print>
#include <sstream>
#include <string>
#include <vector>

#include "rule.hpp"

int parse(std::vector<rule>& parsed_rules, std::vector<std::string>& unique_tokens, std::ifstream& file) {
    using full_rule_type = std::pair<std::vector<std::string>, std::vector<std::string>>;
    std::vector<full_rule_type> full_rules;

    std::string line;
    while (std::getline(file, line)) {
        size_t first_idx = line.find_first_not_of(" \t\r\n");
        if (first_idx == std::string::npos || line[first_idx] == '#') {
            continue;
        }

        std::stringstream ss(std::move(line));
        std::string token;
        bool right_side = false;
        full_rule_type node;
        while (ss >> token) {
            if (token == "->") {
                if (right_side) {
                    std::println(stderr, "В одной строке должна быть ровно одна стрелка, найдена лишняя");
                    return 2;
                }
                right_side = true;
                continue;
            }
            if (right_side) {
                node.second.push_back(token);
            } else {
                node.first.push_back(token);
            }
            unique_tokens.push_back(std::move(token));
        }
        if (!right_side) {
            std::println(stderr, "В одной строке должна быть ровно одна стрелка, ни одной не найдено");
            return 2;
        }
        if (node.first.empty()) {
            std::println(stderr, "Правило должно заменять хотя бы 1 символ");
            return 2;
        }
        full_rules.push_back(std::move(node));
    }
    // TODO: переписать сжатие координат на бор - оптимизация с O(nlogn) до O(n)
    std::sort(unique_tokens.begin(), unique_tokens.end());
    unique_tokens.erase(std::unique(unique_tokens.begin(), unique_tokens.end()), unique_tokens.end());

    parsed_rules.reserve(full_rules.size());
    for (const auto& [left, right] : full_rules) {
        rule node;

        node.left.reserve(left.size());
        for (const auto& str : left) {
            uint32_t idx = std::lower_bound(unique_tokens.begin(), unique_tokens.end(), str) - unique_tokens.begin();
            node.left.push_back(idx);
            if (str[0] == '.') {
                --node.non_term_diff;
            }
        }

        node.right.reserve(right.size());
        for (const auto& str : right) {
            uint32_t idx = std::lower_bound(unique_tokens.begin(), unique_tokens.end(), str) - unique_tokens.begin();
            node.right.push_back(idx);
            if (str[0] == '.') {
                ++node.non_term_diff;
            }
        }

        parsed_rules.push_back(std::move(node));
    }
    return 0;
}