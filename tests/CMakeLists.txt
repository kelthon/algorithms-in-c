# Finds all test files on tests directory
file(GLOB TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/test_*.c")

foreach(test_src ${TEST_SOURCES})
  # Gets tests names without file extension
  get_filename_component(test_name ${test_src} NAME_WE)

  # Creates new test executable with Criterion
  add_executable(${test_name} ${test_src})
  target_link_libraries(${test_name} PRIVATE aic Criterion::Criterion)
  target_include_directories(${test_name} PRIVATE ${PROJECT_SOURCE_DIR}/include)

  # Puts test executables into build/tests/
  set_target_properties(${test_name} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests
  )

  # Adds test to CTest
  add_test(NAME ${test_name} COMMAND ${CMAKE_BINARY_DIR}/tests/${test_name})
endforeach()
