#include <gtest/gtest.h>

#include <sstream>
#include <string>

#include "interpreter.hpp"
#include "parser.hpp"

TEST(ParserTest, BasicParsing) {
    std::string input_string = "d e f";
    std::stringstream ss(
        ".B -> a b c .E\n.B -> a b c .E\n.b -> a b c .b\n.b a b -> a b c .E\n");

    auto result = parse(ss, input_string);

    ASSERT_TRUE(result.has_value()) << "Parser failed: " << result.error();
    EXPECT_EQ(result->parsed_rules.size(), 4);
    EXPECT_EQ(result->token_names.size(), 10);
}

TEST(ParserTest, CommentsAndEmptyLinesIgnored) {
    std::string input_string = "";
    std::stringstream ss(
        "# Comment 1\n\n\n.B -> .E\n\n\n# Comment 2\n.B -> .E\n# Comment 3\n");

    auto result = parse(ss, input_string);

    ASSERT_TRUE(result.has_value()) << "Parser failed: " << result.error();
    EXPECT_EQ(result->parsed_rules.size(), 2);
    EXPECT_EQ(result->token_names.size(), 4);
}

TEST(ParserTest, ParseEmptyRight) {
    std::string input_string = "";
    std::stringstream ss(".B .e ->\n");

    auto result = parse(ss, input_string);

    ASSERT_TRUE(result.has_value()) << "Parser failed: " << result.error();
    EXPECT_EQ(result->parsed_rules.size(), 1);
    EXPECT_EQ(result->token_names.size(), 4);
}

TEST(ParserTest, ParseEmptyGrammar) {
    std::string input_string = "";
    std::stringstream ss("");

    auto result = parse(ss, input_string);

    ASSERT_TRUE(result.has_value()) << "Parser failed: " << result.error();
    EXPECT_TRUE(result->parsed_rules.empty());
    EXPECT_EQ(result->token_names.size(), 4);
}

TEST(ParserTest, InvalidRuleDoubleArrow) {
    std::string input_string = "";
    std::stringstream ss(".B -> -> a\n");

    auto result = parse(ss, input_string);

    EXPECT_FALSE(result.has_value()) << "Expected fail on double arrow";
}

TEST(ParserTest, InvalidRuleNoArrow) {
    std::string input_string = "";
    std::stringstream ss(".B a\n");

    auto result = parse(ss, input_string);

    EXPECT_FALSE(result.has_value()) << "Expected fail on no arrow";
}

TEST(ParserTest, InvalidRuleNoNonTerminals) {
    std::string input_string = "";
    std::stringstream ss("B E -> a\n");

    auto result = parse(ss, input_string);

    EXPECT_FALSE(result.has_value()) << "Expected fail on no non-terminals";
}

TEST(ParserTest, InvalidRuleEmptyLeft) {
    std::string input_string = "";
    std::stringstream ss("-> a\n");

    auto result = parse(ss, input_string);

    EXPECT_FALSE(result.has_value()) << "Expected fail on empty left side";
}

TEST(InterpreterTest, BasicRun) {
    std::string input_string = "";
    std::stringstream ss(".B .E -> .b .e\n");

    auto parse_result = parse(ss, input_string);
    ASSERT_TRUE(parse_result.has_value())
        << "Parser failed: " << parse_result.error();

    auto run_result = run(*parse_result, false);
    ASSERT_TRUE(run_result.has_value())
        << "Interpreter failed: " << run_result.error();

    EXPECT_TRUE(run_result->empty());
}

TEST(InterpreterTest, RunErrorNoApplicableRules) {
    std::string input_string = "";
    std::stringstream ss(".B -> .E\n");

    auto parse_result = parse(ss, input_string);
    ASSERT_TRUE(parse_result.has_value())
        << "Parser failed: " << parse_result.error();

    auto run_result = run(*parse_result, false);
    ASSERT_FALSE(run_result.has_value())
        << "Expected interpreter fail because no applicable rules";
}
