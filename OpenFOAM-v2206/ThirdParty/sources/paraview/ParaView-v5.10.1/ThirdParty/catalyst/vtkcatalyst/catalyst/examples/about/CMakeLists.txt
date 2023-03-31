cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(CATALYST_EXPORT_ABOUT)

find_package(catalyst REQUIRED)

add_executable(catalyst_example_about
  catalyst_example_about.cxx)

target_link_libraries(catalyst_example_about
  PRIVATE catalyst::catalyst)

include(CTest)
if (BUILD_TESTING)
  add_test(
    NAME test
    COMMAND catalyst_example_about --help)
endif()
