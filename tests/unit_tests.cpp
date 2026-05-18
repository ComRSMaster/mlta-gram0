#include <gtest/gtest.h>

#include <sstream>
#include <string>

#include "launcher.hpp"
#include "parser.hpp"

TEST(ParserTest, BasicParsing) {
    std::stringstream input("d e f");
    std::stringstream program(
        ".B -> a b c .E\n.B -> a b c .E\n.b -> a b c .b\n.b a b -> a b c .E\n");

    auto result = parse(program, input);

    ASSERT_TRUE(result.has_value()) << "Parser failed: " << result.error();
    EXPECT_EQ(result->parsed_rules.size(), 4);
    EXPECT_EQ(result->token_names.size(), 10);
}

TEST(ParserTest, CommentsAndEmptyLinesIgnored) {
    std::stringstream input("");
    std::stringstream program(
        "# Comment 1\n\n\n.B -> .E\n\n\n# Comment 2\n.B -> .E\n# Comment 3\n");

    auto result = parse(program, input);

    ASSERT_TRUE(result.has_value()) << "Parser failed: " << result.error();
    EXPECT_EQ(result->parsed_rules.size(), 2);
    EXPECT_EQ(result->token_names.size(), 4);
}

TEST(ParserTest, ParseEmptyRight) {
    std::stringstream input("");
    std::stringstream program(".B .e ->\n");

    auto result = parse(program, input);

    ASSERT_TRUE(result.has_value()) << "Parser failed: " << result.error();
    EXPECT_EQ(result->parsed_rules.size(), 1);
    EXPECT_EQ(result->token_names.size(), 4);
}

TEST(ParserTest, ParseEmptyProgram) {
    std::stringstream input("");
    std::stringstream program("");

    auto result = parse(program, input);

    ASSERT_TRUE(result.has_value()) << "Parser failed: " << result.error();
    EXPECT_TRUE(result->parsed_rules.empty());
    EXPECT_EQ(result->token_names.size(), 4);
}

TEST(ParserTest, InvalidRuleDoubleArrow) {
    std::stringstream input("");
    std::stringstream program(".B -> -> a\n");

    auto result = parse(program, input);

    ASSERT_FALSE(result.has_value()) << "Expected fail on double arrow";
}

TEST(ParserTest, InvalidRuleNoArrow) {
    std::stringstream input("");
    std::stringstream program(".B a\n");

    auto result = parse(program, input);

    ASSERT_FALSE(result.has_value()) << "Expected fail on no arrow";
}

TEST(ParserTest, InvalidRuleNoNonTerminals) {
    std::stringstream input("");
    std::stringstream program("B E -> a\n");

    auto result = parse(program, input);

    ASSERT_FALSE(result.has_value()) << "Expected fail on no non-terminals";
}

TEST(ParserTest, InvalidRuleEmptyLeft) {
    std::stringstream input("");
    std::stringstream program("-> a\n");

    auto result = parse(program, input);

    ASSERT_FALSE(result.has_value()) << "Expected fail on empty left side";
}

TEST(InterpreterTest, BasicLaunch) {
    std::stringstream input("");
    std::stringstream program(".B .E -> .b .e\n");

    auto launch_result = launch(program, input, false);

    ASSERT_TRUE(launch_result.has_value()) << launch_result.error();
    EXPECT_TRUE(launch_result->empty());
}

TEST(InterpreterTest, RunErrorNoApplicableRules) {
    std::stringstream input("");
    std::stringstream program(".B -> .E\n");

    auto launch_result = launch(program, input, false);

    ASSERT_TRUE(!launch_result.has_value() &&
                launch_result.error().starts_with("Execution failed"))
        << "Expected interpreter fail when no applicable rules";
}
