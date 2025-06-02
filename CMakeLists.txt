cmake_minimum_required(VERSION 3.20)
project(algorithms-in-c)

add_compile_options(-Wall -Wextra -Wunused -fsanitize=address,undefined)

include_directories(include)

add_library(aic src/linked_list.c)

add_executable(tests tests/test_linked_list.c)
target_link_libraries(tests aic)

install(TARGETS aic
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(FILES include/linked_list.h DESTINATION include)
