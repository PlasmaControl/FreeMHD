cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(CATALYST_EXAMPLES)

set(examples
    about
    adaptor0)

if(CATALYST_BUILD_REPLAY)
  list(APPEND examples replay)
endif()

if (NOT DEFINED CATALYST_BINARY_DIR)
  find_package(catalyst VERSION 2.0 REQUIRED)
endif()

foreach(example ${examples})
  if (CATALYST_BINARY_DIR)
    add_catalyst_example(${example})
  else()
    add_subdirectory(${example})
  endif()
endforeach()
