set(libb64_sources
    src/cdecode.cpp
    src/cencode.cpp
    )

set(libb64_headers
    include/b64/cdecode.h
    include/b64/cencode.h
    include/b64/decode.h
    include/b64/encode.h
    include/b64/catalyst_b64_mangle.h)

add_library(conduit_b64 OBJECT
  ${libb64_sources})
target_include_directories(conduit_b64
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

c_install_targets(conduit_b64)
