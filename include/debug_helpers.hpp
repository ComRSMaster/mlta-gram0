#pragma once

#include <print>

template <typename... Args>
[[nodiscard]] inline int print_error(std::format_string<Args...> fmt, Args&&... args) {
    std::print(stderr, "\033[1;31m[ ERROR ]\033[0m ");
    std::println(stderr, fmt, std::forward<Args>(args)...);
    return 1;
}
