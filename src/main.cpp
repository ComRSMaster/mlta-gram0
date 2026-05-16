#include "debug_helpers.hpp"
#include "interpreter.hpp"
#include "iostream"
#include "parser.hpp"

int main(int argc, char* argv[]) {
    bool is_verbose = false;
    std::string program_filename;
    std::string input_filename;
    std::string output_filename;

    for (int i = 1; i < argc; ++i) {
        std::string_view arg(argv[i]);
        if (arg == "-v") {
            is_verbose = true;
        } else if (arg == "-i") {
            if (i + 1 < argc) {
                input_filename = argv[i + 1];
                ++i;
            } else {
                return print_error("Missing filename after option '{}'", arg);
            }
        } else if (arg == "-o") {
            if (i + 1 < argc) {
                output_filename = argv[i + 1];
                ++i;
            } else {
                return print_error("Missing filename after option '{}'", arg);
            }
        } else if (arg.starts_with("-")) {
            return print_error("Unknown option '{}'", arg);
        } else {
            if (!program_filename.empty()) {
                return print_error("Multiple program filenames provided");
            }
            program_filename = arg;
        }
    }

    if (argc <= 1) {
        return print_error("Usage: {} program.g0 [-v] [-i input_file] [-o output_file]", argv[0]);
    }

    if (program_filename.empty()) {
        return print_error("Missing program filename");
    }

    std::ifstream program_file(program_filename);
    if (!program_file) {
        return print_error("Cannot open file '{}'", program_filename);
    }

    std::string input_string;
    if (input_filename.empty()) {
        std::getline(std::cin, input_string);
    } else {
        std::ifstream input_file(input_filename);
        if (!input_file) {
            return print_error("Cannot open file '{}'", input_filename);
        }
        std::getline(input_file, input_string);
    }

    std::vector<rule> parsed_rules;
    std::vector<uint32_t> parsed_input_string;
    std::vector<std::string> token_names;
    int non_term_bal = 0;
    int status_code = parse(parsed_rules, non_term_bal, input_string, parsed_input_string, token_names, program_file);
    if (status_code) {
        return status_code;
    }

    std::string result_string;
    status_code = run(parsed_rules, non_term_bal, parsed_input_string, result_string, token_names, is_verbose);
    if (status_code) {
        return status_code;
    }

    if (output_filename.empty()) {
        std::println("{}", result_string);
    } else {
        std::ofstream output_file(output_filename);
        if (!output_file) {
            return print_error("Cannot open file '{}'", output_filename);
        }
        output_file << result_string << "\n";
    }
}
