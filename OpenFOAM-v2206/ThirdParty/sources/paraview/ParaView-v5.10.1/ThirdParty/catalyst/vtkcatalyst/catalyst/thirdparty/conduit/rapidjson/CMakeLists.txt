add_library(conduit_rapidjson INTERFACE)
target_include_directories(conduit_rapidjson
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
c_install_targets(conduit_rapidjson)
