#pragma once

#define sign(x) ((x) > 0) - ((x) < 0)

#ifdef DEBUG
    #include <cstdio>
    #include <cstdarg>
    inline void LOG_(const char* file, const char* func, int line, const char* format, ...) {
        va_list args;
        va_start(args, format);
        printf("[%s:%s:%d]: ", file, func, line);
        vprintf(format, args);
        va_end(args);
    }

    #define LOG(args...) LOG_(__FILE__, __func__, __LINE__, args)
#else
    inline void LOG(...) {}
#endif
