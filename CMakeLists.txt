cmake_minimum_required(VERSION 3.20)
project(mlta-gram0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(gram
    src/main.cpp
    src/parser.cpp
    src/interpreter.cpp
)
