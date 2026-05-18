#pragma once

#include <expected>
#include <string>

std::expected<std::string, std::string> launch(std::istream& program_stream,
                                               std::istream& input_stream,
                                               bool is_verbose);