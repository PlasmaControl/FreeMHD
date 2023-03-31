set(fmt_headers
    conduit_fmt/chrono.h
    conduit_fmt/color.h
    conduit_fmt/compile.h
    conduit_fmt/conduit_fmt.h
    conduit_fmt/core.h
    conduit_fmt/format-inl.h
    conduit_fmt/format.h
    conduit_fmt/locale.h
    conduit_fmt/os.h
    conduit_fmt/ostream.h
    conduit_fmt/posix.h
    conduit_fmt/printf.h
    conduit_fmt/ranges.h
    )

add_library(conduit_fmt INTERFACE)
  # XXX(cmake-3.19)
  #${fmt_headers})
target_include_directories(conduit_fmt
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

c_install_targets(conduit_fmt)
