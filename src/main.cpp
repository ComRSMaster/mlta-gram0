#include <print>

#include "interpreter.cpp"
#include "parser.cpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::println(stderr, "Использование: {} <путь к программе>", argv[0]);
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file) {
        std::println(stderr, "Не удалось открыть файл {}", argv[1]);
        return 1;
    }

    std::vector<rule> parsed_rules;
    std::vector<std::string> token_names;
    int code = parse(parsed_rules, token_names, file);
    if (code) {
        return code;
    }

    // for (const auto& str: token_names) {
    //     std::println("{}", str);
    // }
    // for (const auto& rl: parsed_rules) {
    //     std::println("{} -> {}", rl.left[0], rl.right[0]);
    // }

    return run(parsed_rules, token_names, true);
}
