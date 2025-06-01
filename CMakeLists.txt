cmake_minimum_required(VERSION 3.15)

project(
        clockwork
        VERSION 0.1
        DESCRIPTION "HCE Community Engine"
        LANGUAGES CXX)

add_executable(clockwork src/main.cpp src/uci.cpp src/uci.hpp src/square.hpp src/move.hpp src/common.hpp src/util/types.hpp)

set_target_properties(clockwork PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO)

# Warnings
# We don't support MSVC
target_compile_options(clockwork PRIVATE -Wall -Wextra -Wconversion)

target_compile_options(clockwork PRIVATE
 $<$<CONFIG:Debug>:
    -fsanitize=address,undefined -g3
  >)

target_link_options(clockwork PRIVATE
 $<$<CONFIG:Debug>:
    -fsanitize=address,undefined
  >)

#Optimizations
target_compile_options(clockwork PRIVATE -march=native)

# LTO
include(CheckIPOSupported)
check_ipo_supported(RESULT lto)
if(lto)
  set_target_properties(clockwork PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()
