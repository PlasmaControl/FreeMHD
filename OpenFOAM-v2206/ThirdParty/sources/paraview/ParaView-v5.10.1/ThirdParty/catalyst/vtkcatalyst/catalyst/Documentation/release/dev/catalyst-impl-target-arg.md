## `catalyst_implementation(CATALYST_TARGET)` argument

The `catalyst_implementation` CMake function now accepts a `CATALYST_TARGET`
argument to use instead of the `catalyst::catalyst` target if it needs to be
used outside of the scope where `find_package(catalyst)` is called.
