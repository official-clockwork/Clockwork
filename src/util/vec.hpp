#pragma once

#if (__AVX512F__ && (__AVX512BW__ || __AVX512VNNI__))
    #include "util/vec/avx512.hpp"
#else
    #include "util/vec/avx2.hpp"
#endif
