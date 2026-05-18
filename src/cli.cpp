#include "cli.hpp"

#include <expected>
#include <format>

std::expected<cli_args, std::string> parse_cli_args(int argc, char* argv[]) {
    cli_args args;

    for (int i = 1; i < argc; ++i) {
        std::string_view arg(argv[i]);
        if (arg == "-v") {
            args.is_verbose = true;
        } else if (arg == "-i") {
            if (i + 1 < argc) {
                args.input_filename = argv[i + 1];
                ++i;
            } else {
                return std::unexpected(
                    std::format("Missing filename after option '{}'", arg));
            }
        } else if (arg == "-o") {
            if (i + 1 < argc) {
                args.output_filename = argv[i + 1];
                ++i;
            } else {
                return std::unexpected(
                    std::format("Missing filename after option '{}'", arg));
            }
        } else if (arg.starts_with("-")) {
            return std::unexpected(std::format("Unknown option '{}'", arg));
        } else {
            if (!args.program_filename.empty()) {
                return std::unexpected(
                    std::format("Multiple program filenames provided"));
            }
            args.program_filename = arg;
        }
    }

    if (args.program_filename.empty()) {
        return std::unexpected("Missing program filename");
    }

    return args;
}
