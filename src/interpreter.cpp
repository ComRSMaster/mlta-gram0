#include "interpreter.hpp"

#include <algorithm>
#include <cstdint>
#include <print>
#include <string>
#include <vector>

#include "debug_helpers.hpp"
#include "rule.hpp"

std::string concat_tokens(const std::vector<uint32_t>& str, const std::vector<std::string>& token_names) {
    if (str.empty()) {
        return "";
    }

    std::string result;
    for (size_t i = 0; i < str.size() - 1; ++i) {
        result += token_names[str[i]] + " ";
    }
    result += token_names[str.back()];

    return result;
}

template <typename... Args>
inline void print_verbose(std::format_string<Args...> fmt, Args&&... args) {
    std::print(stderr, "\033[1;33m[ VERBOSE ]\033[0m ");
    std::println(stderr, fmt, std::forward<Args>(args)...);
}

[[nodiscard]] int run(const std::vector<rule>& parsed_rules, uint32_t non_term_bal, std::vector<uint32_t>& current_string, std::string& result_string, const std::vector<std::string>& token_names, bool is_verbose) {
    if (is_verbose) {
        print_verbose("Start string: {}\n", concat_tokens(current_string, token_names));
    }

    uint32_t idx_B = std::lower_bound(token_names.begin(), token_names.end(), ".b") - token_names.begin();
    uint32_t idx_E = std::lower_bound(token_names.begin(), token_names.end(), ".e") - token_names.begin();

    while (non_term_bal != 2 || current_string[0] != idx_B || current_string.back() != idx_E) {
        bool changed = false;
        for (const auto& cur_rule : parsed_rules) {
            auto it = std::search(current_string.begin(), current_string.end(), cur_rule.left.begin(), cur_rule.left.end());
            if (it == current_string.end()) {
                continue;
            }
            auto new_it = current_string.erase(it, it + cur_rule.left.size());
            current_string.insert(new_it, cur_rule.right.begin(), cur_rule.right.end());
            non_term_bal += cur_rule.non_term_diff;
            changed = true;
            if (is_verbose) {
                print_verbose("Applied rule: {} -> {}", concat_tokens(cur_rule.left, token_names), concat_tokens(cur_rule.right, token_names));
                print_verbose("Current string: {}\n", concat_tokens(current_string, token_names));
            }
            break;
        }
        if (!changed) {
            return print_error("Cannot apply any rules. Current string: {}", concat_tokens(current_string, token_names));
        }
    }

    current_string.erase(current_string.begin());
    current_string.pop_back();
    result_string = concat_tokens(current_string, token_names);
    return 0;
}
