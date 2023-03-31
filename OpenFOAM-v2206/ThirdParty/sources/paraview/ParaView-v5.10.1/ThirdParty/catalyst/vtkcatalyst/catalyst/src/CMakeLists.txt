add_subdirectory(catalyst)

option(CATALYST_BUILD_STUB_IMPLEMENTATION "Enable stub implementation" ON)
if (CATALYST_BUILD_STUB_IMPLEMENTATION)
  add_subdirectory(impl-stub)
endif ()

if (CATALYST_BUILD_TOOLS)
  add_subdirectory(tools)
endif ()
