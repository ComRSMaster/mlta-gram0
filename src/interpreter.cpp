#include <algorithm>
#include <cstdint>
#include <print>
#include <string>
#include <vector>

#include "rule.hpp"

void print_string(const std::vector<uint32_t>& str, const std::vector<std::string>& token_names) {
    if (str.empty()) {
        std::println();
        return;
    }
    for (size_t i = 0; i < str.size() - 1; ++i) {
        std::print("{} ", token_names[str[i]]);
    }
    std::println("{}", token_names[str.back()]);
}

int run(const std::vector<rule>& parsed_rules, const std::vector<std::string>& token_names, bool show_trace = false) {
    uint32_t idx = std::lower_bound(token_names.begin(), token_names.end(), ".") - token_names.begin();
    std::vector<uint32_t> current{idx};

    uint32_t non_term_bal = 1;

    if (show_trace) {
        print_string(current, token_names);
    }

    while (non_term_bal) {
        bool changed = false;
        for (const auto& cur_rule : parsed_rules) {
            // TODO: переписать на бор
            auto it = std::search(current.begin(), current.end(), cur_rule.left.begin(), cur_rule.left.end());
            if (it == current.end()) {
                continue;
            }
            auto new_it = current.erase(it, it + cur_rule.left.size());
            current.insert(new_it, cur_rule.right.begin(), cur_rule.right.end());
            non_term_bal += cur_rule.non_term_diff;
            changed = true;
            if (show_trace) {
                print_string(current, token_names);
            }
        }
        if (!changed) {
            std::println(stderr, "Программа никогда не завершится, невозможно применить ни одно правило.");
            return 3;
        }
    }
    print_string(current, token_names);
    return 0;
}
