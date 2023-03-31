# CMakeLists for Catalyst

configure_file ("${CMAKE_CURRENT_SOURCE_DIR}/conduit_blueprint_exports.h.in"
                "${CMAKE_CURRENT_BINARY_DIR}/conduit_blueprint_exports.h")


#
# Specify all headers
#
set(blueprint_headers
    conduit_blueprint.hpp
    conduit_blueprint_mesh.hpp
    conduit_blueprint_mesh_utils.hpp
    conduit_blueprint_mesh_examples.hpp
    conduit_blueprint_mesh_examples_julia.hpp
    conduit_blueprint_mesh_examples_venn.hpp
    conduit_blueprint_mcarray.hpp
    conduit_blueprint_mcarray_examples.hpp
    conduit_blueprint_o2mrelation.hpp
    conduit_blueprint_o2mrelation_utils.hpp
    conduit_blueprint_o2mrelation_iterator.hpp
    conduit_blueprint_o2mrelation_examples.hpp
    conduit_blueprint_zfparray.hpp
    ${CMAKE_CURRENT_BINARY_DIR}/conduit_blueprint_exports.h
)

#
# Specify blueprint c sources
#
set(blueprint_c_headers
    c/catalyst_conduit_blueprint_mangle.h
    c/conduit_blueprint.h
    c/conduit_blueprint_c_exports.h
    c/conduit_blueprint_mcarray.h
    c/conduit_blueprint_mesh.h
    )


#
# Specify blueprint cpp sources
#
set(blueprint_sources
    conduit_blueprint.cpp
    conduit_blueprint_mesh.cpp
    conduit_blueprint_mesh_utils.cpp
    conduit_blueprint_mesh_matset_xforms.cpp
    conduit_blueprint_mesh_examples.cpp
    conduit_blueprint_mesh_examples_julia.cpp
    conduit_blueprint_mesh_examples_venn.cpp
    conduit_blueprint_mcarray.cpp
    conduit_blueprint_mcarray_examples.cpp
    conduit_blueprint_o2mrelation.cpp
    conduit_blueprint_o2mrelation_utils.cpp
    conduit_blueprint_o2mrelation_iterator.cpp
    conduit_blueprint_o2mrelation_examples.cpp
    conduit_blueprint_zfparray.cpp
    )

#
# Specify blueprint c sources
#
set(blueprint_c_sources
    c/conduit_blueprint_c.cpp
    c/conduit_blueprint_mcarray_c.cpp
    c/conduit_blueprint_mesh_c.cpp
    )

add_library(blueprint_headers INTERFACE)
add_library(catalyst::blueprint_headers ALIAS blueprint_headers)
target_include_directories(blueprint_headers
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/c>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:${CATALYST_INSTALL_INCLUDE_DIR}>)
c_install_headers(
  HEADERS ${blueprint_c_headers})
c_install_targets(blueprint_headers)

#-------------------------------------------------------------------------
# build blueprint library
add_library(blueprint OBJECT
  ${blueprint_sources}
  ${blueprint_c_sources})
target_include_directories(blueprint
  PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../conduit>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(blueprint
  PUBLIC
    catalyst::blueprint_headers
    catalyst::conduit)
if (BUILD_SHARED_LIBS)
  target_compile_definitions(blueprint
    PRIVATE
      conduit_c_EXPORTS
      conduit_blueprint_c_EXPORTS)
endif ()
add_library(catalyst::blueprint ALIAS blueprint)
c_install_targets(blueprint)
