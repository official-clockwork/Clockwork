cmake_minimum_required(VERSION 3.15)

project(clockwork
        VERSION 0.1
        DESCRIPTION "HCE Community Engine"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_OSX_ARCHITECTURES "x86_64")

option(CLOCKWORK_ENABLE_EVALTUNE "Enable building of evaltune executable" OFF)

#Optimizations
set(CLOCKWORK_MARCH_TARGET "native" CACHE STRING "Specify the target architecture for -march (e.g., native, skylake, znver2). Set to OFF or empty to disable.")

if (CLOCKWORK_MARCH_TARGET AND NOT CLOCKWORK_MARCH_TARGET STREQUAL "OFF")
    message(STATUS "Using -march=${CLOCKWORK_MARCH_TARGET} for optimizations")
    add_compile_options("-march=${CLOCKWORK_MARCH_TARGET}")
else ()
    message(STATUS "-march flag disabled by user")
endif()

# LTO
include(CheckIPOSupported)
check_ipo_supported(RESULT lto)

# Flags
function(target_add_flags target)

    # C++ standard version
    set_target_properties(${target} PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO)

    # Warnings
    # We don't support MSVC
    target_compile_options(${target} PUBLIC -Wall -Wextra -Wconversion)

    target_compile_options(${target} PUBLIC
        $<$<CONFIG:Debug>:
        -fsanitize=address,undefined -g3
        -D_GLIBCXX_DEBUG -D_LIBCPP_DEBUG=1
        >)

    target_link_options(${target} PUBLIC
        $<$<CONFIG:Debug>:
        -fsanitize=address,undefined
        >)

    # LTO
    if (lto)
        set_target_properties(${target} PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
    endif()

endfunction()

# Sorted list of source files
set(srcs
        src/board.cpp
        src/board.hpp
        src/bench.cpp
        src/bench.hpp
        src/common.hpp
        src/evaluation.cpp
        src/evaluation.hpp
        src/eval_constants.hpp
        src/eval_types.hpp
        src/geometry.cpp
        src/geometry.hpp
        src/history.cpp
        src/history.hpp
        src/move.cpp
        src/move.hpp
        src/movegen.cpp
        src/movegen.hpp
        src/movepick.cpp
        src/movepick.hpp
        src/perft.cpp
        src/perft.hpp
        src/position.cpp
        src/position.hpp
        src/psqt_state.hpp
        src/repetition_info.hpp
        src/repetition_info.cpp
        src/search.cpp
        src/search.hpp
        src/square.hpp
        src/tm.cpp
        src/tm.hpp
        src/tt.cpp
        src/tt.hpp
        src/tuned.cpp
        src/tuned.hpp
        src/uci.cpp
        src/uci.hpp
        src/zobrist.cpp
        src/zobrist.hpp
        src/util/bit.hpp
        src/util/parse.hpp
        src/util/pretty.hpp
        src/util/static_vector.hpp
        src/util/types.hpp
        src/util/vec.hpp
        src/util/vec/avx2.hpp
        src/util/vec/avx512.hpp
        src/tuning/globals.hpp
        src/tuning/graph.cpp
        src/tuning/graph.hpp
        src/tuning/loss.hpp
        src/tuning/optim.hpp
        src/tuning/value.cpp
        src/tuning/value.hpp
)

if(CLOCKWORK_ENABLE_EVALTUNE)

    add_executable(clockwork-evaltune src/evaltune_main.cpp ${srcs})
    target_include_directories(clockwork-evaltune PUBLIC src)
    target_compile_options(clockwork-evaltune PUBLIC -DEVAL_TUNING=1)
    target_add_flags(clockwork-evaltune)

else()

    add_library(clockwork-lib OBJECT ${srcs})
    target_include_directories(clockwork-lib PUBLIC src)
    target_add_flags(clockwork-lib)

    add_executable(clockwork src/main.cpp)
    target_link_libraries(clockwork PUBLIC clockwork-lib)
    target_add_flags(clockwork)

    # Tests
    enable_testing()
    set(CMAKE_SKIP_TEST_ALL_DEPENDENCY FALSE)
    function(do_test name)
        add_executable(${name} tests/${name}.cpp)
        target_link_libraries(${name} PUBLIC clockwork-lib)
        target_add_flags(${name})
        add_test(${name} ${name})
    endfunction()

    do_test(test_piece_count)
    do_test(test_see)
    do_test(test_see_prototype)
    do_test(test_is_legal)
    do_test(test_repetition)
    do_test(test_static_vector)
    do_test(test_perft)
    do_test(test_position)

endif()
