cmake_minimum_required(VERSION 3.15)

project(
        clockwork
        VERSION 0.1
        DESCRIPTION "HCE Community Engine"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_OSX_ARCHITECTURES "x86_64")
include_directories(src)

#Optimizations
set(CLOCKWORK_MARCH_TARGET "native" CACHE STRING "Specify the target architecture for -march (e.g., native, skylake, znver2). Set to OFF or empty to disable.")

if(CLOCKWORK_MARCH_TARGET AND NOT CLOCKWORK_MARCH_TARGET STREQUAL "OFF")
    message(STATUS "Using -march=${CLOCKWORK_MARCH_TARGET} for optimizations")
    add_compile_options("-march=${CLOCKWORK_MARCH_TARGET}")
else()
    message(STATUS "-march flag disabled by user")
endif()

# Sorted list of source files
set(srcs
    src/board.cpp
    src/board.hpp
    src/common.hpp
    src/geometry.cpp
    src/geometry.hpp
    src/move.cpp
    src/move.hpp
    src/movegen.cpp
    src/movegen.hpp
    src/perft.cpp
    src/perft.hpp
    src/position.cpp
    src/position.hpp
    src/search.cpp
    src/search.hpp
    src/square.hpp
    src/uci.cpp
    src/uci.hpp
    src/util/bit.hpp
    src/util/static_vector.hpp
    src/util/types.hpp
    src/util/vec.hpp
    src/util/vec/avx2.hpp
    src/util/vec/avx512.hpp
)

add_executable(clockwork ${srcs} src/main.cpp)
set_target_properties(clockwork PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO)

# Tests
enable_testing()
set(CMAKE_SKIP_TEST_ALL_DEPENDENCY FALSE)
function(do_test name)
  add_executable(${name} tests/${name}.cpp ${srcs})
  add_test(${name} ${name})
endfunction()

do_test(test_perft)
do_test(test_position)

# Warnings
# We don't support MSVC
target_compile_options(clockwork PRIVATE -Wall -Wextra -Wconversion)

target_compile_options(clockwork PRIVATE
        $<$<CONFIG:Debug>:
        -fsanitize=address,undefined -g3
        -D_GLIBCXX_DEBUG -D_LIBCPP_DEBUG=1
        >)

target_link_options(clockwork PRIVATE
        $<$<CONFIG:Debug>:
        -fsanitize=address,undefined
        >)

# LTO
include(CheckIPOSupported)
check_ipo_supported(RESULT lto)
if (lto)
    set_target_properties(clockwork PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif ()
