# Copyright (c) Lawrence Livermore National Security, LLC and other Conduit
# Project developers. See top-level LICENSE AND COPYRIGHT files for dates and
# other details. No copyright assignment is required to contribute to Conduit.

# XXX(kitware) this file has been cleaned up to only set variables we care about
# when updating it's probably best to scan through the new version of this file
# and then update accordingly.

if(WIN32 AND BUILD_SHARED_LIBS)
    set(CONDUIT_WINDOWS_DLL_EXPORTS TRUE)
endif()

# XXX(kitware) this variable is not set in Conduit, adding it to
# remove dependence on PROJECT_VERSION in conduit_config.h
set(CONDUIT_VERSION "0.7.2")
set(CONDUIT_USE_CXX11 1)
set(CONDUIT_INSTALL_PREFIX "")
set(CONDUIT_MAKE_EXTRA_LIBS "")
set(CONDUIT_GIT_SHA1 "f54f834eb8aaff4fc97613e04cfdb360997867be")

###############################################################################
# This macro converts a cmake path to a platform specific string literal
# usable in C++. (For example, on windows C:/Path will be come C:\\Path)
###############################################################################

macro(convert_to_native_escaped_file_path path output)
    file(TO_NATIVE_PATH ${path} ${output})
    string(REPLACE "\\" "\\\\"  ${output} "${${output}}")
endmacro()
