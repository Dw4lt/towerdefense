#pragma once

#define sign(x) ((x) > 0) - ((x) < 0)

#ifdef DEBUG
    #include <cstdio>
    #include <cstdarg>
    inline void LOG(const char* format, ...) {
        va_list args;
        va_start(args, format);
        printf("[%s:%s:%d]: ", __FILE__, __func__, __LINE__);
        vprintf(format, args);
        va_end(args);
    }
#else
    inline void LOG(...) {}
#endif
