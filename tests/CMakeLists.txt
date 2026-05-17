find_package(GTest REQUIRED)

add_executable(unit_tests
    unit_tests.cpp
)
target_link_libraries(unit_tests PRIVATE gram_lib GTest::gtest_main)

gtest_discover_tests(unit_tests)

add_executable(integration_tests
    integration_tests.cpp
)
target_link_libraries(integration_tests PRIVATE gram_lib GTest::gtest_main)

gtest_discover_tests(integration_tests)
