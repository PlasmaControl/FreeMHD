# CMakeLists for Catalyst

include(CMakeBasics)
include(BasicTypeChecks)
include("cmake/BitwidthMapping.cmake")

configure_file ("${CMAKE_CURRENT_SOURCE_DIR}/conduit_config.h.in"
                "${CMAKE_CURRENT_BINARY_DIR}/conduit_config.h")

configure_file ("${CMAKE_CURRENT_SOURCE_DIR}/conduit_bitwidth_style_types.h.in"
                "${CMAKE_CURRENT_BINARY_DIR}/conduit_bitwidth_style_types.h")

configure_file ("${CMAKE_CURRENT_SOURCE_DIR}/conduit_exports.h.in"
                "${CMAKE_CURRENT_BINARY_DIR}/conduit_exports.h")

#
# Specify conduit cpp headers
#
set(conduit_headers
    conduit.hpp
    conduit_core.hpp
    conduit_endianness.hpp
    conduit_data_array.hpp
    conduit_data_type.hpp
    conduit_node.hpp
    conduit_generator.hpp
    conduit_error.hpp
    conduit_node_iterator.hpp
    conduit_schema.hpp
    conduit_log.hpp
    conduit_utils.hpp
    conduit_config.hpp
    )

#
# Specify conduit c interface headers
#
set(conduit_c_headers
    c/conduit.h
    c/conduit_c_exports.h
    c/conduit_datatype.h
    c/conduit_node.h
    c/conduit_cpp_to_c.hpp
    c/conduit_utils.h
    c/catalyst_conduit_mangle.h

    # Not in the `/c/` directory, but needed transitively.
    ${CMAKE_CURRENT_BINARY_DIR}/conduit_bitwidth_style_types.h
    ${CMAKE_CURRENT_BINARY_DIR}/conduit_config.h
    ${CMAKE_CURRENT_BINARY_DIR}/conduit_exports.h
    ${CMAKE_CURRENT_SOURCE_DIR}/conduit_endianness_types.h
    )

#
# Specify conduit cpp sources
#
set(conduit_sources
    conduit_core.cpp
    conduit_error.cpp
    conduit_endianness.cpp
    conduit_data_type.cpp
    conduit_data_array.cpp
    conduit_generator.cpp
    conduit_node.cpp
    conduit_node_iterator.cpp
    conduit_schema.cpp
    conduit_log.cpp
    conduit_utils.cpp
    )

#
# Specify conduit c interface sources
#
set(conduit_c_sources
    c/conduit_c.cpp
    c/conduit_datatype_c.cpp
    c/conduit_node_c.cpp
    c/conduit_cpp_to_c.cpp
    c/conduit_utils_c.cpp
    )

#-------------------------------------------------------------------------
# an interface target to simply setup include paths.
add_library(conduit_headers INTERFACE)
add_library(catalyst::conduit_headers ALIAS conduit_headers)
target_include_directories(conduit_headers
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/c>
    $<INSTALL_INTERFACE:${CATALYST_INSTALL_INCLUDE_DIR}>)
c_install_headers(
  HEADERS ${conduit_c_headers})
c_install_targets(conduit_headers)

#-------------------------------------------------------------------------
# build conduit library
add_library(conduit OBJECT
    ${conduit_sources}
    ${conduit_c_sources})
target_include_directories(conduit
  PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(conduit
  PUBLIC
    catalyst::conduit_headers
  PRIVATE
    conduit_fmt
    conduit_libyaml
    conduit_b64
    conduit_rapidjson)
if (BUILD_SHARED_LIBS)
  target_compile_definitions(conduit
    PRIVATE
      conduit_c_EXPORTS)
endif ()
add_library(catalyst::conduit ALIAS conduit)
c_install_targets(conduit)
