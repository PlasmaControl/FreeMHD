# CMakeLists for Catalyst

#-------------------------------------------------------------------------
list(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

#-------------------------------------------------------------------------
add_subdirectory(conduit)
add_subdirectory(blueprint)

#-------------------------------------------------------------------------
# Add subdirectories for third-party dependencies of conduit
add_subdirectory(fmt)
add_subdirectory(libyaml)
add_subdirectory(libb64)
add_subdirectory(rapidjson)
