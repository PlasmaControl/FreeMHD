if(POLICY CMP0028)
  cmake_policy(SET CMP0028 NEW)
endif()

if(POLICY CMP0057)
  cmake_policy(SET CMP0057 NEW)
endif()

if(POLICY CMP0074)
  cmake_policy(SET CMP0074 NEW)
endif()

set(ADIOS2_HAVE_Fortran )
set(ADIOS2_HAVE_MPI TRUE)

function(adios2_add_thirdparty_target PackageName)
  add_library(adios2::thirdparty::${PackageName} INTERFACE IMPORTED)
endfunction()

# Add default find components.
set(_ADIOS2_FIND_COMPONENTS_DEFAULT "")
if(NOT ";${${CMAKE_FIND_PACKAGE_NAME}_FIND_COMPONENTS};" MATCHES ";(C|CXX|Fortran);")
  # No language components were requested.  Add enabled languages by default.
  foreach(lang C CXX Fortran)
    if(CMAKE_${lang}_COMPILER_LOADED)
      list(APPEND _ADIOS2_FIND_COMPONENTS_DEFAULT "${lang}")
    endif()
    if(${CMAKE_FIND_PACKAGE_NAME}_FIND_REQUIRED)
      set(${CMAKE_FIND_PACKAGE_NAME}_FIND_REQUIRED_${lang} TRUE)
    endif()
  endforeach()
endif()
if(NOT ${CMAKE_FIND_PACKAGE_NAME}_FIND_COMPONENTS)
  # No components were requested.  Add MPI by default if available.
  if(ADIOS2_HAVE_MPI)
    list(APPEND _ADIOS2_FIND_COMPONENTS_DEFAULT "MPI")
    if(${CMAKE_FIND_PACKAGE_NAME}_FIND_REQUIRED)
      set(${CMAKE_FIND_PACKAGE_NAME}_FIND_REQUIRED_MPI TRUE)
    endif()
  endif()
endif()
list(APPEND ${CMAKE_FIND_PACKAGE_NAME}_FIND_COMPONENTS ${_ADIOS2_FIND_COMPONENTS_DEFAULT})
unset(_ADIOS2_FIND_COMPONENTS_DEFAULT)

# Language components require the corresponding language to be enabled.
if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/adios2-c-targets.cmake")
  set(${CMAKE_FIND_PACKAGE_NAME}_C_FOUND ${CMAKE_C_COMPILER_LOADED})
else()
  set(${CMAKE_FIND_PACKAGE_NAME}_C_FOUND FALSE)
endif()
if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/adios2-cxx11-targets.cmake")
  set(${CMAKE_FIND_PACKAGE_NAME}_CXX_FOUND ${CMAKE_CXX_COMPILER_LOADED})
else()
  set(${CMAKE_FIND_PACKAGE_NAME}_CXX_FOUND FALSE)
endif()
if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/adios2-fortran-targets.cmake")
  set(${CMAKE_FIND_PACKAGE_NAME}_Fortran_FOUND ${CMAKE_Fortran_COMPILER_LOADED})
else()
  set(${CMAKE_FIND_PACKAGE_NAME}_Fortran_FOUND FALSE)
endif()

include(CMakeFindDependencyMacro)

if(NOT ON)
  # Ensure C is enabled for use by package dependencies found below.
  enable_language(C)
  # Ensure CXX is enabled so we can link against the C++ runtime library.
  enable_language(CXX)
endif()

if(ADIOS2_HAVE_MPI AND "MPI" IN_LIST ${CMAKE_FIND_PACKAGE_NAME}_FIND_COMPONENTS)
  find_dependency(MPI)
  set(${CMAKE_FIND_PACKAGE_NAME}_MPI_FOUND ${MPI_FOUND})
else()
  set(${CMAKE_FIND_PACKAGE_NAME}_MPI_FOUND FALSE)
endif()

if(NOT ON)
  set(ADIOS2_HAVE_Blosc )
  if(ADIOS2_HAVE_Blosc)
    find_dependency(Blosc)
  endif()

  set(ADIOS2_HAVE_BZip2 TRUE)
  if(ADIOS2_HAVE_BZip2)
    find_dependency(BZip2)
  endif()

  set(ADIOS2_HAVE_ZFP )
  if(ADIOS2_HAVE_ZFP)
    find_dependency(ZFP)
  endif()

  set(ADIOS2_HAVE_SZ )
  if(ADIOS2_HAVE_SZ)
    find_dependency(SZ)
  endif()
  
  set(ADIOS2_HAVE_MGARD )
  if(ADIOS2_HAVE_MGARD)
    find_dependency(MGARD)
  endif()

  set(ADIOS2_HAVE_PNG TRUE)
  if(ADIOS2_HAVE_PNG)
    find_dependency(PNG)
  endif()

  set(ADIOS2_HAVE_DataSpaces )
  if(ADIOS2_HAVE_DataSpaces)
    find_dependency(DataSpaces)
  endif()

  set(ADIOS2_HAVE_ZeroMQ )
  if(ADIOS2_HAVE_ZeroMQ)
    find_dependency(ZeroMQ)
  endif()

  set(ADIOS2_HAVE_HDF5 TRUE)
  if(ADIOS2_HAVE_HDF5)
    find_dependency(HDF5 COMPONENTS C)
  endif()

  adios2_add_thirdparty_target(pugixml)
  set(ADIOS2_USE_EXTERNAL_PUGIXML OFF)
  if(ADIOS2_USE_EXTERNAL_PUGIXML)
    find_dependency(pugixml 1.10)
    target_link_libraries(adios2::thirdparty::pugixml INTERFACE pugixml)
  endif()

  adios2_add_thirdparty_target(yaml-cpp)
  set(ADIOS2_USE_EXTERNAL_YAMLCPP OFF)
  if(ADIOS2_USE_EXTERNAL_YAMLCPP)
    find_dependency(yaml-cpp)
    target_link_libraries(adios2::thirdparty::yaml-cpp INTERFACE yaml-cpp)
  endif()

  # No concrete implementation, just put this here to satisfy quasi-dependency
  adios2_add_thirdparty_target(nlohmann_json)

  set(ADIOS2_HAVE_SST TRUE)
  if(ADIOS2_HAVE_SST)
    set(ADIOS2_SST_HAVE_LIBFABRIC )
    if(ADIOS2_SST_HAVE_LIBFABRIC)
      find_dependency(LIBFABRIC)
    endif()

    find_dependency(EVPath)
    add_library(adios2::thirdparty::EVPath INTERFACE IMPORTED)
    set_target_properties(adios2::thirdparty::EVPath PROPERTIES
      INTERFACE_LINK_LIBRARIES EVPath::EVPath
    )
  endif()

endif()

include("${CMAKE_CURRENT_LIST_DIR}/adios2-config-version.cmake")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(${CMAKE_FIND_PACKAGE_NAME} CONFIG_MODE HANDLE_COMPONENTS)

if(NOT TARGET adios2::core)
  include("${CMAKE_CURRENT_LIST_DIR}/adios2-targets.cmake")
  include("${CMAKE_CURRENT_LIST_DIR}/adios2-c-targets.cmake" OPTIONAL)
  include("${CMAKE_CURRENT_LIST_DIR}/adios2-cxx11-targets.cmake" OPTIONAL)
  include("${CMAKE_CURRENT_LIST_DIR}/adios2-fortran-targets.cmake" OPTIONAL)

  add_library(adios2::adios2 INTERFACE IMPORTED)
  foreach(_adios2_tgt adios2::c adios2::c_mpi adios2::cxx11 adios2::cxx11_mpi)
    if(TARGET "${_adios2_tgt}")
      set_property(TARGET adios2::adios2 PROPERTY INTERFACE_LINK_LIBRARIES "${_adios2_tgt}" APPEND)
    endif()
  endforeach()

  # Filter our usage requirements to avoid non-enabled languages.
  if(NOT CMAKE_C_COMPILER_LOADED)
    # Since C is not available, take out C bindings from legacy library.
    get_property(_adios2_link_libs TARGET adios2::adios2 PROPERTY INTERFACE_LINK_LIBRARIES)
    list(FILTER _adios2_link_libs EXCLUDE REGEX "^adios2::(c|c_mpi)$")
    set_property(TARGET adios2::adios2 PROPERTY INTERFACE_LINK_LIBRARIES "${_adios2_link_libs}")
    unset(_adios2_link_libs)
    if(TARGET adios2::core_mpi)
      # Since C is not available, use MPI through MPI_CXX instead.
      get_property(_adios2_link_libs TARGET adios2::core_mpi PROPERTY INTERFACE_LINK_LIBRARIES)
      string(REGEX REPLACE "MPI::MPI_C(;|>|$)" "MPI::MPI_CXX\\1" _adios2_link_libs "${_adios2_link_libs}")
      set_property(TARGET adios2::core_mpi PROPERTY INTERFACE_LINK_LIBRARIES "${_adios2_link_libs}")
      unset(_adios2_link_libs)
    endif()
  endif()
  if(NOT CMAKE_CXX_COMPILER_LOADED)
    # Since CXX is not available, take out CXX bindings from legacy library.
    get_property(_adios2_link_libs TARGET adios2::adios2 PROPERTY INTERFACE_LINK_LIBRARIES)
    list(FILTER _adios2_link_libs EXCLUDE REGEX "^adios2::(cxx11|cxx11_mpi)$")
    set_property(TARGET adios2::adios2 PROPERTY INTERFACE_LINK_LIBRARIES "${_adios2_link_libs}")
    unset(_adios2_link_libs)
  endif()
endif()

# backwards compatibility for things that still use package variables
set(${CMAKE_FIND_PACKAGE_NAME}_INCLUDE_DIRS)
set(${CMAKE_FIND_PACKAGE_NAME}_LIBRARIES adios2::adios2)
