#include <istream>

#include "launcher.hpp"
#include "interpreter.hpp"
#include "parser.hpp"

std::expected<std::string, std::string> launch(std::istream& program_stream,
                                               std::istream& input_stream,
                                               bool is_verbose) {
    auto parse_result = parse(program_stream, input_stream);
    if (!parse_result) {
        return std::unexpected(std::format("Parse failed: {}", parse_result.error()));
    }

    auto run_result = run(*parse_result, is_verbose);
    if (!run_result) {
        return std::unexpected(std::format("Execution failed: {}", run_result.error()));
    }

    return *run_result;
}
