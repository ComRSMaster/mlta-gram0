#include <fstream>

#include "cli.hpp"
#include "debug_helpers.hpp"
#include "iostream"
#include "launcher.hpp"

int main(int argc, char* argv[]) {
    auto args = parse_cli_args(argc, argv);
    if (!args) {
        return print_error(
            "Args parsing failed: {}\n\n"
            "Usage: {} program.g0 [-v] [-i input_file] [-o output_file]",
            args.error(), argv[0]);
    }

    std::ifstream program_file(args->program_filename);
    if (!program_file) {
        return print_error("Cannot open program file '{}'",
                           args->program_filename);
    }

    std::istream* input_stream = &std::cin;
    std::ifstream input_file;
    if (!args->input_filename.empty()) {
        input_file.open(args->input_filename);
        if (!input_file) {
            return print_error("Cannot open input file '{}'",
                               args->input_filename);
        }
        input_stream = &input_file;
    }

    auto launch_result = launch(program_file, *input_stream, args->is_verbose);
    if (!launch_result) {
        return print_error("{}", launch_result.error());
    }

    if (args->output_filename.empty()) {
        std::println("{}", *launch_result);
    } else {
        std::ofstream output_file(args->output_filename);
        if (!output_file) {
            return print_error("Cannot open output file '{}'",
                               args->output_filename);
        }
        output_file << *launch_result << "\n";
    }
}
