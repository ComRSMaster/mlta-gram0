#include "interpreter.hpp"

#include <algorithm>
#include <cstdint>
#include <expected>
#include <string>
#include <vector>

#include "debug_helpers.hpp"
#include "parser.hpp"
#include "rule.hpp"

std::string concat_tokens(
    const std::vector<uint32_t>& str,
    const std::vector<std::string>& token_names) {
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

[[nodiscard]] std::expected<std::string, std::string> run(
    const parse_result& parsed_data, bool is_verbose) {
    std::vector<uint32_t> current_string = parsed_data.parsed_input_string;
    int non_term_bal = parsed_data.non_term_bal;

    if (is_verbose) {
        print_verbose("Start string: {}\n",
                      concat_tokens(current_string, parsed_data.token_names));
    }

    uint32_t idx_B = std::lower_bound(parsed_data.token_names.begin(),
                                      parsed_data.token_names.end(), ".b") -
                     parsed_data.token_names.begin();
    uint32_t idx_E = std::lower_bound(parsed_data.token_names.begin(),
                                      parsed_data.token_names.end(), ".e") -
                     parsed_data.token_names.begin();

    while (non_term_bal != 2 || current_string[0] != idx_B ||
           current_string.back() != idx_E) {
        bool changed = false;
        for (const auto& cur_rule : parsed_data.parsed_rules) {
            auto it = std::search(current_string.begin(), current_string.end(),
                                  cur_rule.left.begin(), cur_rule.left.end());
            if (it == current_string.end()) {
                continue;
            }
            auto new_it = current_string.erase(it, it + cur_rule.left.size());
            current_string.insert(new_it, cur_rule.right.begin(),
                                  cur_rule.right.end());
            non_term_bal += cur_rule.non_term_diff;
            changed = true;
            if (is_verbose) {
                print_verbose(
                    "Applied rule: {} -> {}",
                    concat_tokens(cur_rule.left, parsed_data.token_names),
                    concat_tokens(cur_rule.right, parsed_data.token_names));
                print_verbose(
                    "Current string: {}\n",
                    concat_tokens(current_string, parsed_data.token_names));
            }
            break;
        }
        if (!changed) {
            return std::unexpected(std::format(
                "Cannot apply any rules. Current string: {}",
                concat_tokens(current_string, parsed_data.token_names)));
        }
    }

    current_string.pop_back();
    current_string.erase(current_string.begin());

    return concat_tokens(current_string, parsed_data.token_names);
}
