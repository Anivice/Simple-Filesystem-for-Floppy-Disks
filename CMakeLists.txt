cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME        swplayer)
set(PROJECT_VERSION     0.0.1)
project(${PROJECT_NAME} C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE "Debug")

if (NOT (CMAKE_COMPILER_IS_GNUCC AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 10.2.0) )
    message(FATAL_ERROR "You have to use GCC (ver >= 10.2.0) as the compiler!")
endif()

enable_testing()

include(modules/compile_definition.cmake)
include(modules/quick_add_test.cmake)
include(modules/add_utils.cmake)

include(FindPkgConfig)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBELF  REQUIRED libelf)

add_compile_definitions("PACKAGE_NAME=\"${PROJECT_NAME}\"")
add_compile_definitions("PACKAGE_VERSION=\"${PROJECT_VERSION}\"")

# enable code coverage
add_compile_options(-fprofile-arcs -ftest-coverage -O0 --coverage)
add_link_options(--coverage)

add_library(swplayer STATIC
        src/include/debug.h
        src/debug/elf_process.cpp
        src/debug/invoke_addr2line.cpp
        src/debug/simple_err.cpp    src/include/simple_err.h
        src/include/wav.h
        )
target_include_directories(swplayer PUBLIC src/include)
target_link_libraries(swplayer      PUBLIC ${LIBELF_LIBRARIES})
target_link_libraries(swplayer      PUBLIC gcov)

target_include_directories(swplayer PUBLIC ${LIBELF_INCLUDE_DIRS})
target_compile_options(swplayer     PUBLIC ${LIBELF_CFLAGS_OTHER})

add_executable(sw_player src/main.cpp)
target_link_libraries(sw_player PUBLIC swplayer)

_add_test(ftrace    "function trace system")
