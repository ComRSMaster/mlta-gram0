#include <gtest/gtest.h>

#include <fstream>
#include <sstream>
#include <string>

#include "launcher.hpp"

std::string int_to_binary_string(int n, int length) {
    std::string result;
    for (int i = length - 1; i >= 0; --i) {
        result += ((n & (1 << i)) ? "1 " : "0 ");
    }
    if (!result.empty()) {
        result.pop_back();
    }
    return result;
}

TEST(IntegrationTest, IsEvenPalindrome) {
    std::ifstream program("examples/is_even_palindrome.g0");
    ASSERT_TRUE(program) << "Cannot open program file from examples directory";

    for (int len = 1; len < 14; ++len) {
        for (int n = 0; n < (1 << len); ++n) {
            std::string input_str = int_to_binary_string(n, len);
            std::string reversed_str = input_str;
            std::reverse(reversed_str.begin(), reversed_str.end());

            std::string expected_str =
                (input_str == reversed_str && len % 2 == 0) ? "1" : "";

            std::stringstream input(input_str);
            program.clear();
            program.seekg(0);
            auto launch_result = launch(program, input, false);

            ASSERT_TRUE(launch_result.has_value()) << launch_result.error();
            EXPECT_EQ(*launch_result, expected_str)
                << "Failed on input: " << input_str;
        }
    }
}

TEST(IntegrationTest, InvertBinaryString) {
    std::ifstream program("examples/str_invert.g0");
    ASSERT_TRUE(program) << "Cannot open program file from examples directory";

    for (int len = 0; len < 15; ++len) {
        for (int n = 0; n < (1 << len); ++n) {
            std::string input_str = int_to_binary_string(n, len);

            std::string expected_str =
                int_to_binary_string(((1 << len) - 1) ^ n, len);

            std::stringstream input(input_str);
            program.clear();
            program.seekg(0);
            auto launch_result = launch(program, input, false);

            ASSERT_TRUE(launch_result.has_value()) << launch_result.error();
            EXPECT_EQ(*launch_result, expected_str)
                << "Failed on input: " << input_str;
        }
    }
}

TEST(IntegrationTest, ReverseBinaryString) {
    std::ifstream program("examples/str_reverse.g0");
    ASSERT_TRUE(program) << "Cannot open program file from examples directory";

    for (int len = 0; len < 14; ++len) {
        for (int n = 0; n < (1 << len); ++n) {
            std::string input_str = int_to_binary_string(n, len);

            std::string expected_str = input_str;
            std::reverse(expected_str.begin(), expected_str.end());

            std::stringstream input(input_str);
            program.clear();
            program.seekg(0);
            auto launch_result = launch(program, input, false);

            ASSERT_TRUE(launch_result.has_value()) << launch_result.error();
            EXPECT_EQ(*launch_result, expected_str)
                << "Failed on input: " << input_str;
        }
    }
}

TEST(IntegrationTest, CountToN) {
    std::ifstream program("examples/count_to.g0");
    ASSERT_TRUE(program) << "Cannot open program file from examples directory";

    std::string expected_str = "";

    for (int n = 0; n < (1 << 6); ++n) {
        std::string input_str =
            int_to_binary_string(n, std::__lg(std::max(1, n)) + 1);

        expected_str += input_str;

        std::stringstream input(input_str);
        program.clear();
        program.seekg(0);
        auto launch_result = launch(program, input, false);

        ASSERT_TRUE(launch_result.has_value()) << launch_result.error();
        EXPECT_EQ(*launch_result, expected_str)
            << "Failed on input: " << input_str;

        expected_str += " , ";
    }
}
