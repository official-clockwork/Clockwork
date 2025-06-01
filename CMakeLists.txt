cmake_minimum_required(VERSION 3.15)

project(
        clockwork
        VERSION 0.1
        DESCRIPTION "HCE Community Engine"
        LANGUAGES CXX)

add_executable(clockwork src/main.cpp src/uci.cpp src/uci.hpp src/util/types.h)

set_target_properties(clockwork PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO)

# Warnings
# We don't support MSVC
target_compile_options(clockwork PRIVATE -Wall -Wextra -Wconversion)

#Optimizations
target_compile_options(clockwork PRIVATE -march=native)

# LTO
include(CheckIPOSupported)
check_ipo_supported(RESULT lto)
if(lto)
  set_target_properties(clockwork PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()
