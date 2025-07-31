#pragma once

#include <cstdlib>
#include <iostream>
#include <source_location>

#define REQUIRE(x)                                                                             \
    do {                                                                                       \
        if (!(x)) {                                                                            \
            std::cout << "Assertion failed at line " << std::source_location::current().line() \
                      << ": " #x;                                                              \
            std::exit(1);                                                                      \
        }                                                                                      \
    } while (false)
