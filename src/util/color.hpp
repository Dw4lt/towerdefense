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

enum Colors {
    // Baloon colors
    BALOON_RED = RGB_888_TO_565(0xFF'03'03),
    BALOON_BLUE = RGB_888_TO_565(0x03'03'FF),
    BALOON_GREEN = RGB_888_TO_565(0x03'FF'03),
    BALOON_YELLOW = RGB_888_TO_565(0xE0'DA'19),
    BALOON_PINK = RGB_888_TO_565(0xFF'14'93),

    // Primitive colors
    WHITE_ = RGB_888_TO_565(0xE0E0E0), // TODO: Issue - libndls includes a macro named "WHITE" :(
    RED = RGB_888_TO_565(0xFF0000),
    BROWN = RGB_888_TO_565(0x8B4513),
    DARK_GREEN = RGB_888_TO_565(0x023020),
    DARK_ORANGE = RGB_888_TO_565(0xff5900),
    INTENSE_BLUE = RGB_888_TO_565(0x344ceb),
    INTENSE_YELLOW = RGB_888_TO_565(0xebd234),
    INTENSE_ORANGE = RGB_888_TO_565(0xFF'8C'00),
    LIGHT_BLUE = RGB_888_TO_565(0x137BF0),
    LIGHTER_BLUE = RGB_888_TO_565(0x5197FF),
    LIGHTER_GREEN = RGB_888_TO_565(0x38761d),

    // Color mapping
    PATH_COLOR = BROWN,
    CURSOR_ON_PATH = DARK_ORANGE,
    CURSOR_ON_BUILDING = INTENSE_YELLOW,
    CURSOR_ON_EMPTY_FIELD_TILE = INTENSE_BLUE,
    UNKNOWN = INTENSE_ORANGE,
    ARCHER_IDLE = LIGHT_BLUE,
    ARCHER_FIRING = LIGHTER_BLUE,
    SNIPER_IDLE = DARK_GREEN,
    SNIPER_FIRING = LIGHTER_GREEN,
    SHOP_BACKGROUND = WHITE_,
    SHOP_CURSOR = RED,
};
