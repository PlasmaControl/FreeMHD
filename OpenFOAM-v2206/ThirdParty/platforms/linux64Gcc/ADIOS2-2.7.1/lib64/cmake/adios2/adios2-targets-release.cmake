#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "adios2::core" for configuration "Release"
set_property(TARGET adios2::core APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(adios2::core PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "adios2::taustubs"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib64/libadios2_core.so.2.7.1"
  IMPORTED_SONAME_RELEASE "libadios2_core.so.2"
  )

list(APPEND _IMPORT_CHECK_TARGETS adios2::core )
list(APPEND _IMPORT_CHECK_FILES_FOR_adios2::core "${_IMPORT_PREFIX}/lib64/libadios2_core.so.2.7.1" )

# Import target "adios2::core_mpi" for configuration "Release"
set_property(TARGET adios2::core_mpi APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(adios2::core_mpi PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "adios2::taustubs"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib64/libadios2_core_mpi.so.2.7.1"
  IMPORTED_SONAME_RELEASE "libadios2_core_mpi.so.2"
  )

list(APPEND _IMPORT_CHECK_TARGETS adios2::core_mpi )
list(APPEND _IMPORT_CHECK_FILES_FOR_adios2::core_mpi "${_IMPORT_PREFIX}/lib64/libadios2_core_mpi.so.2.7.1" )

# Import target "adios2::taustubs" for configuration "Release"
set_property(TARGET adios2::taustubs APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(adios2::taustubs PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib64/libadios2_taustubs.so"
  IMPORTED_SONAME_RELEASE "libadios2_taustubs.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS adios2::taustubs )
list(APPEND _IMPORT_CHECK_FILES_FOR_adios2::taustubs "${_IMPORT_PREFIX}/lib64/libadios2_taustubs.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
