#ifndef COLOR_CONVERSION
#define COLOR_CONVERSION

#define RGB_888_TO_565(x) ((x & 0xF8) >> 3) + ((x & 0xfC00) >> 5) + ((x & 0xf80000) >> 8)
#define RGB_565_TO_888(x) ((x & 0x3F) << 3) + ((x & 0x7f00) << 5) + ((x & 0x3f0000) << 8)


#endif