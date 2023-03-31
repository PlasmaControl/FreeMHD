#------------------------------------------------------------------------------
# Get build directory functional

set(_binary_package_dir "${CMAKE_BINARY_DIR}/${CATALYST_INSTALL_PACKAGE_DIR}")

# These are targets for users of Catalyst
# i.e the simulation codes.
export(
  EXPORT    Catalyst
  NAMESPACE catalyst::
  FILE      ${_binary_package_dir}/catalyst-targets.cmake)

include(CMakePackageConfigHelpers)
write_basic_package_version_file("${_binary_package_dir}/catalyst-config-version.cmake"
  VERSION       "${CATALYST_VERSION}"
  COMPATIBILITY SameMajorVersion)

configure_package_config_file(
  "${CMAKE_CURRENT_LIST_DIR}/catalyst-config.cmake.in"
  "${_binary_package_dir}/catalyst-config.cmake"
  INSTALL_DESTINATION "${CATALYST_INSTALL_PACKAGE_DIR}")


configure_file("${CMAKE_CURRENT_LIST_DIR}/catalyst-macros.cmake"
               "${_binary_package_dir}/catalyst-macros.cmake" COPYONLY)
configure_file("${CMAKE_CURRENT_LIST_DIR}/catalyst_impl.c.in"
               "${_binary_package_dir}/catalyst_impl.c.in" COPYONLY)
configure_file("${CMAKE_CURRENT_LIST_DIR}/catalyst_impl.h.in"
               "${_binary_package_dir}/catalyst_impl.h.in" COPYONLY)
#------------------------------------------------------------------------------
# Get install directory functional

# We only install targes for users of Catalyst i.e. simulation codes
install(
  EXPORT      Catalyst
  NAMESPACE   catalyst::
  FILE        catalyst-targets.cmake
  DESTINATION "${CATALYST_INSTALL_PACKAGE_DIR}")

install(
  FILES       "${_binary_package_dir}/catalyst-config-version.cmake"
              "${_binary_package_dir}/catalyst-config.cmake"
              "${_binary_package_dir}/catalyst-macros.cmake"
              "${_binary_package_dir}/catalyst_impl.c.in"
              "${_binary_package_dir}/catalyst_impl.h.in"
  DESTINATION "${CATALYST_INSTALL_PACKAGE_DIR}")
