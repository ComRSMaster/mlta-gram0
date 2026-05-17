add_library(gram_lib
    parser.cpp
    interpreter.cpp
)

add_executable(gram
    main.cpp
)

target_link_libraries(gram PRIVATE gram_lib)
