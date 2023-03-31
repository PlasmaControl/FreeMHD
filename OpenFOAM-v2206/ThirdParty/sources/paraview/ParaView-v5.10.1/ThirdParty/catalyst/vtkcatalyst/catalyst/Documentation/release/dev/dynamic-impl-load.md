## Dynamically load Catalyst implmentations

The `catalyst` library is now an actual library rather than an interface.
Implementations will need to provide a structure with a specific name, though
the CMake API for creating a Catalyst implementation will do this
automatically.

To select an implementation, the `catalyst_load/implementation` and
`catalyst_load/search_paths/*` settings passed into `catalyst_initialize` are
used to select and find the implementation library and load it. As a fallback,
the environment variables `CATALYST_IMPLEMENTATION_NAME` and
`CATALYST_IMPLEMENTATION_PATHS` are used as the implementation name as a
list of paths to search for implementations, respectively.

The `CATALYST_IMPLEMENTATION_` environment variables may be instead preferred
over the `catalyst_load/` settings by setting the
`CATALYST_IMPLEMENTATION_PREFER_ENV` environment variable to a non-empty
string.

Instead of the `catalyst_library` CMake API applying properties to an existing
target, there is now `catalyst_implementation` to create an implementation
given a target and implementation name. The actual implementation will also
need to be added to the target either through the `SOURCES` argument or using
`target_sources`.

  - All Catalyst API functions now return an error code. This is primarily to
    allow `catalyst_initialize` to return the status of the backing
    implementation.
  - There is now a structure containing the functions that are needed for
    Catalyst.
