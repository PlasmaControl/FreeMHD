# - Config file for the Dill package
#
# It defines the following variables
#   DILL_INCLUDE_DIRS - include directories for Dill
#   DILL_LIBRARIES    - libraries to link against
#   DILL_HAS_NATIVE_DCG - Whether or not native code generation is available
#
# And the folloing imported targets:
#   dill::dill
#
 
include(CMakeFindDependencyMacro)

# Locate dependencies if building static
set(_dill_required_vars)
if(NOT ON)
  if(TRUE)
    find_dependency(LibFFI)
    list(APPEND _dill_required_vars LibFFI_FOUND)
  endif()
  if(NOT TRUE)
    find_dependency(BinUtils)
    list(APPEND _dill_required_vars BinUtils_FOUND)
  endif()
endif()

if(_dill_required_vars)
  list(INSERT _dill_required_vars 0 REQUIRED_VARS)
endif()

include("${CMAKE_CURRENT_LIST_DIR}/dill-config-version.cmake")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(${CMAKE_FIND_PACKAGE_NAME}
  CONFIG_MODE ${_dill_required_vars})

if(NOT TARGET dill::dill)
  include("${CMAKE_CURRENT_LIST_DIR}/dill-targets.cmake")
endif()

set(DILL_LIBRARIES dill::dill)
set(DILL_INCLUDE_DIRS
  $<TARGET_PROPERTY:dill::dill,INTERFACE_INCLUDE_DIRECTORIES>
)
set(DILL_HAS_NATIVE_DCG  TRUE)
set(DILL_HAS_EMULATION   TRUE)
set(DILL_HAS_DISASSEMBLY OFF)
