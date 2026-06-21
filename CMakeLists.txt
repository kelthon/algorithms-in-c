cmake_minimum_required(VERSION 4.1)
project(algorithms-in-c VERSION 1.0 LANGUAGES C)

# Set the required C standard to C17 (ISO/IEC 9899:2018)
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Set the outputs directories in build folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Configure global compiler warnings and strictness
add_compile_options(-Wall -Wextra -Wpedantic -Wconversion -Werror)

# Compiling all source files into a aic library
file(GLOB SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
add_library(obj_target OBJECT ${SRC_FILES})
target_include_directories(obj_target PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(aic STATIC $<TARGET_OBJECTS:obj_target>)

# Enables testing and adds tests directory
enable_testing()

# Find and configure Criterion
find_package(Criterion REQUIRED)

add_subdirectory(tests)

# 1. Configure installation rules for the library and headers
# install(TARGETS aic
#    EXPORT aic-targets
#    LIBRARY DESTINATION lib
#    ARCHIVE DESTINATION lib
# )

# 2. Configure installation rules for the header files
# install(FILES include/linked_list.h DESTINATION include)

# 3. This step generates the necessary metadata for other CMake projects to find and use 
# the 'aic' library using 'find_package(algorithms-in-c)'.
# install(EXPORT aic-targets
#    FILE 
#        algorithms-in-cConfig.cmake
#    NAMESPACE aic::
#    DESTINATION 
#        lib/cmake/algorithms-in-c
# )
