#include <fstream>

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
        return print_error(
            "Usage: {} program.g0 [-v] [-i input_file] [-o output_file]",
            argv[0]);
    }

    if (program_filename.empty()) {
        return print_error("Missing program filename");
    }

    std::ifstream program_file(program_filename);
    if (!program_file) {
        return print_error("Cannot open program file '{}'", program_filename);
    }

    std::string input_string;
    if (input_filename.empty()) {
        std::getline(std::cin, input_string);
    } else {
        std::ifstream input_file(input_filename);
        if (!input_file) {
            return print_error("Cannot open input file '{}'", input_filename);
        }
        std::getline(input_file, input_string);
    }

    auto parse_result = parse(program_file, input_string);
    if (!parse_result) {
        return print_error("{}", parse_result.error());
    }

    auto run_result = run(*parse_result, is_verbose);
    if (!run_result) {
        return print_error("{}", run_result.error());
    }

    if (output_filename.empty()) {
        std::println("{}", *run_result);
    } else {
        std::ofstream output_file(output_filename);
        if (!output_file) {
            return print_error("Cannot open output file '{}'", output_filename);
        }
        output_file << *run_result << "\n";
    }
}
