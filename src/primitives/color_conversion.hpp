#ifndef COLOR_CONVERSION
#define COLOR_CONVERSION

constexpr int RGB_888_TO_565(int x) {
    return ((x & 0xF8) >> 3) + ((x & 0xfC00) >> 5) + ((x & 0xf80000) >> 8);
}

constexpr int RGB_565_TO_888(int x) {
    return ((x & 0x3F) << 3) + ((x & 0x7f00) << 5) + ((x & 0x3f0000) << 8);
}

constexpr int PSEUDO_RANDOM_SEEDED(int x, int y) {  // Gives back a pseudo-random looking number based on the coords
    return (((x * 5 + y * 7 + 351) * 795) % (x * y + 3 * (x + y)));
}

#endif
