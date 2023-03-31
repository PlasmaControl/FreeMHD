cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(CATALYST_EXAMPLE_ADAPTOR0)

# When implementing the Catalyst API, as against using
# it to invoke Catalyst, one must use the component
# `SDK` in `find_package` call. This ensures that all necessary
# libraries etc. are available.
find_package(catalyst
  REQUIRED
  COMPONENTS SDK)

# use this function call to mark any library as the
# Catalyst API implementation.
catalyst_implementation(
  TARGET  catalyst_example_adaptor0
  NAME    example_adaptor0
  SOURCES catalyst_example_adaptor0.cxx)

include(CTest)
if (BUILD_TESTING)
  # Add a test that links against the standard Catalyst library
  # However at runtime ensures the implementation built here
  # is being used.
  add_executable(adaptor0_test
    adaptor0_test.cxx)
  target_link_libraries(adaptor0_test
    PRIVATE
      catalyst::catalyst)
  add_test(
    NAME adaptor0_test
    COMMAND adaptor0_test "$<TARGET_FILE_DIR:catalyst_example_adaptor0>")
endif()
