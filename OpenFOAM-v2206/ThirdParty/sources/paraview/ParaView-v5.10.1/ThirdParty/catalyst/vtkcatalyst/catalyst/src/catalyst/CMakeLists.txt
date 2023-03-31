set(headers
  catalyst.h
  catalyst_api.h
  catalyst_impl.h
  catalyst_stub.h
  catalyst.hpp
  catalyst_conduit.hpp
  catalyst_conduit_blueprint.hpp
  catalyst_conduit_error.hpp
  ${CMAKE_CURRENT_BINARY_DIR}/catalyst_export.h
  ${CMAKE_CURRENT_BINARY_DIR}/catalyst_version.h)
configure_file(catalyst_version.h.in catalyst_version.h)

set(private_headers
  catalyst_dump_node.hpp)

#------------------------------------------------------------
# Add an interface library just to setup include dirs.
add_library(catalyst_headers INTERFACE)
target_include_directories(catalyst_headers
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:${CATALYST_INSTALL_INCLUDE_DIR}>)
add_library(catalyst::catalyst_headers ALIAS catalyst_headers)

if (WIN32)
  set_property(SOURCE catalyst_api.c APPEND
    PROPERTY
      COMPILE_DEFINITIONS "CATALYST_DEBUG_SUFFIX=\"$<$<CONFIG:Debug>:d>\"")
endif ()

add_library(catalyst
  catalyst_api.c
  catalyst_api_default.cpp
  catalyst_stub.cpp
  ${headers}
  ${private_headers})
target_compile_features(catalyst
  PRIVATE
    c_std_99)
set_target_properties(catalyst
  PROPERTIES
    # This should never need to be bumped. libcatalyst is intended to be stable
    # in its ABI, so removing APIs is not something we should be doing. This
    # includes exposed Conduit APIs.
    SOVERSION 3)
target_link_libraries(catalyst
  PUBLIC
    catalyst::blueprint_headers
    catalyst::conduit_headers
    catalyst::catalyst_headers
  PRIVATE
    conduit
    blueprint
    conduit_libyaml
    conduit_b64
    ${CMAKE_DL_LIBS})

if (WIN32)
  set_property(TARGET catalyst
    PROPERTY
      DEBUG_POSTFIX d)
endif ()

if (BUILD_SHARED_LIBS)
  target_compile_definitions(catalyst
    PRIVATE
      conduit_c_EXPORTS
      conduit_blueprint_c_EXPORTS)
endif ()

add_library(catalyst::catalyst ALIAS catalyst)

if (CATALYST_USE_MPI)
  target_link_libraries(catalyst PRIVATE MPI::MPI_C)
  target_compile_definitions(catalyst
    PRIVATE CATALYST_USE_MPI
            # force `mpi.h` to not include cxx components to avoid
            # unnecessary dependency on MPI_CPP
            MPICH_SKIP_MPICXX
            OMPI_SKIP_MPICXX
            MPI_NO_CPPBIND
            _MPICC_H)
endif ()

c_install_targets(catalyst catalyst_headers)
c_install_headers(
  HEADERS ${headers})

#-------------------------------------------------------------
# Generate export header.
set_target_properties(catalyst
  PROPERTIES
    DEFINE_SYMBOL catalyst_EXPORTS)
include(GenerateExportHeader)
generate_export_header(catalyst)
