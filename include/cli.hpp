#pragma once

#include <expected>
#include <string>

struct cli_args {
    std::string program_filename;
    std::string input_filename;
    std::string output_filename;
    bool is_verbose = false;
};

std::expected<cli_args, std::string> parse_cli_args(int argc, char* argv[]);
