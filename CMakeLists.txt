
# set this will supress some warnings
set(BUILDING_SDK "yes" CACHE INTERNAL "")

# basic config
if (NOT PROJ)
    get_filename_component(PROJ ${CMAKE_CURRENT_BINARY_DIR} DIRECTORY)
    get_filename_component(PROJ ${PROJ} NAME)
    string(REPLACE " " "_" PROJ ${PROJ})
    message(STATUS "PROJ not set, use ${PROJ} as PROJ. Also, you can set it manually. e.g. -DPROJ=hello_world")
else()
    message("PROJ = ${PROJ}")
endif ()

cmake_minimum_required(VERSION 3.0)
include(./tools/cmake/common.cmake)
project(${PROJ} C CXX ASM)

# config self use headers
include(./tools/cmake/macros.internal.cmake)
#header_directories(src/${PROJ})
# build library first
# add_subdirectory(lib)

# compile project
#add_source_files(src/${PROJ}/*.c src/${PROJ}/*.s src/${PROJ}/*.S src/${PROJ}/*.cpp)
message("${CMAKE_CURRENT_SOURCE_DIR}")
include(./sdk.cmake)

#include(./cmake/executable.cmake)
include(./tools/cmake/dump-config.cmake)


