# set this will supress some warnings
set(BUILDING_SDK "yes" CACHE INTERNAL "")

# basic config
if (NOT PROJ)
    message(STATUS "PROJ not set, use ${PROJ} as PROJ. Also, you can set it manually. e.g. -DPROJ=hello_world")
    set(PROJ blink)
else()
    message("PROJ = ${PROJ}")
endif ()

# basic config
if (NOT BOARD)
    message(STATUS "BOARD not set, use ${BOARD} as BOARD. Also, you can set it manually. e.g. -DBOARD=arch_mix")
else()
    message("BOARD = ${BOARD}")
endif ()


cmake_minimum_required(VERSION 3.0)
include(./tools/cmake/common.cmake)
project(${PROJ} C CXX ASM)

# config self use headers
include(./tools/cmake/macros.internal.cmake)
header_directories(libraries/SPI)
header_directories(libraries/Wire)
header_directories(variants/${BOARD})
header_directories(examples/${PROJ})

# build library first
add_source_file(libraries/SPI/*.cpp)
add_source_file(libraries/Wire/*.cpp)

FILE(GLOB_RECURSE PROJ_LIB_SRC_INO
    "${CMAKE_CURRENT_SOURCE_DIR}/examples/${PROJ}/*.ino"
    )
SET_PROPERTY(SOURCE ${PROJ_LIB_SRC_INO} PROPERTY LANGUAGE CXX)
SET_SOURCE_FILES_PROPERTIES(${PROJ_LIB_SRC_INO} PROPERTIES COMPILE_FLAGS "-x c++")

# compile project
add_source_files(examples/${PROJ}/*.c examples/${PROJ}/*.s examples/${PROJ}/*.S examples/${PROJ}/*.cpp)


include(./sdk.cmake)
include(./cmake/executable.cmake)
include(./tools/cmake/dump-config.cmake)

target_sources(${PROJECT_NAME} PUBLIC ${PROJ_LIB_SRC_INO})



