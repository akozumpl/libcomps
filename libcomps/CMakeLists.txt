project(libcomps)

cmake_minimum_required (VERSION 2.6)
include (${CMAKE_ROOT}/Modules/CheckFunctionExists.cmake)
include (${CMAKE_SOURCE_DIR}/version.cmake)
#set (CMAKE_CXX_COMPILER g++)

set (CMAKE_C_FLAGS          "${CMAKE_C_FLAGS} --pedantic -std=c99 -Wall -Wextra -g -Wno-missing-field-initializers")
set (CMAKE_C_FLAGS_DEBUG    "${CMAKE_C_FLAGS} -ggdb -O0 -Wall -Wextra")

set (VERSION "${libcomps_VERSION_MAJOR}.${libcomps_VERSION_MINOR}.${libcomps_VERSION_PATCH}")
#set (libcomps_SOURCE comps_list.c)
#set (libcomps_HEADERS comps_list.h)

include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/src")
#include_directories("${PROJECT_SOURCE_DIR}/src/libcomps")

FIND_LIBRARY(CHECK_LIBRARY NAMES check)
FIND_LIBRARY(EXPAT_LIBRARY NAMES expat)

find_package(LibXml2 REQUIRED)
find_package(EXPAT REQUIRED)

include_directories(${CHECK_INCLUDE_DIR})
include_directories(${EXPAT_INCLUDE_DIR})
include_directories(${LIBXML2_INCLUDE_DIR})

add_subdirectory (src)
add_subdirectory (tests)
add_subdirectory (docs)
