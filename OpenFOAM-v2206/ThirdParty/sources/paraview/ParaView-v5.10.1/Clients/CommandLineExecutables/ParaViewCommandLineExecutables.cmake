function (paraview_add_executable name)
  # Set up rpaths
  set(CMAKE_BUILD_RPATH_USE_ORIGIN 1)
  if (UNIX AND NOT APPLE)
    file(RELATIVE_PATH _paraview_client_relpath
      "/prefix/${CMAKE_INSTALL_BINDIR}"
      "/prefix/${CMAKE_INSTALL_LIBDIR}")
    set(_paraview_client_origin_rpath
      "$ORIGIN/${_paraview_client_relpath}")

    list(APPEND CMAKE_INSTALL_RPATH
      "${_paraview_client_origin_rpath}")
  endif()

  add_executable("${name}" ${ARGN})
  add_executable("ParaView::${name}" ALIAS "${name}")

  target_link_libraries("${name}"
    PRIVATE
      ParaView::RemotingApplication)

  target_link_libraries("${name}"
    PRIVATE
      ParaView::paraview_plugins)

  if (PARAVIEW_USE_PYTHON)
    target_compile_definitions("${name}"
      PRIVATE
        PARAVIEW_USE_PYTHON)
    target_link_libraries("${name}"
      PRIVATE
        VTK::PythonInterpreter
        ParaView::PythonInitializer)
  endif ()

  if (paraview_exe_job_link_pool)
    set_property(TARGET "${name}"
      PROPERTY
        JOB_POOL_LINK "${paraview_exe_job_link_pool}")
  endif ()

  install(
    TARGETS     "${name}"
    DESTINATION "${CMAKE_INSTALL_BINDIR}"
    COMPONENT   runtime
    EXPORT      ParaViewTools)
endfunction ()
