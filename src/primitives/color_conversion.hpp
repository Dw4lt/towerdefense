#pragma once

constexpr int RGB_888_TO_565(int x) {
    return ((x & 0xF8) >> 3) | ((x & 0xfC00) >> 5) | ((x & 0xf80000) >> 8);
}

constexpr int RGB_888_TO_565(int r, int g, int b) {
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3);
}

constexpr int PSEUDO_RANDOM_SEEDED(int x, int y) {  // Gives back a pseudo-random looking number based on the coords
    return (((x * 5 + y * 7 + 351) * 795) % (x * y + 3 * (x + y)));
}
