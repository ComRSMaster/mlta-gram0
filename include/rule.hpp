#pragma once

#include <cstdint>
#include <vector>

struct rule {
    std::vector<uint32_t> left;
    std::vector<uint32_t> right;
    int non_term_diff = 0;  // non_term_right - non_term_left
};
