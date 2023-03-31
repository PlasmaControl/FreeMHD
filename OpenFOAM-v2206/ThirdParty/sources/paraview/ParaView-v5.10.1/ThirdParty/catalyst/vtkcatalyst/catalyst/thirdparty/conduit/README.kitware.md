# conduit fork for Catalyst

This branch contains changes required to embed conduit into Catalyst. This
includes changes made primarily to the build system to allow it to be embedded
into another source tree as well as a header to facilitate mangling of the
symbols to avoid conflicts with other copies of the library within a single
process.

  * Ignore whitespace errors for Catalyst's commit checks.
  * Fix file permissions.
  * Simplify CMakeBasics.cmake to remove unnecessary bits.
  * Write a CMake build system for use in Catalyst.
  * Mangle all symbols to avoid conflicts.
