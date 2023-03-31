set(_CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH})
list(INSERT CMAKE_MODULE_PATH 0 "/home/bw0594/3DLMM/OpenFOAM-v2206/ThirdParty/sources/adios/ADIOS2-2.7.1/thirdparty/EVPath/EVPath/cmake")

include("${CMAKE_CURRENT_LIST_DIR}/EVPathConfigCommon.cmake")

set(CMAKE_MODULE_PATH ${_CMAKE_MODULE_PATH})
unset(_CMAKE_MODULE_PATH)
