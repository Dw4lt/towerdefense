#include "input.hpp"
#include "SDL/SDL_timer.h"

namespace Input {
    int actionRepetitionOnLongPress(Uint32& previous_timestamp, int& previous_button_state, int current_button_state, Uint32 repetition_timer_ms) {
        if (current_button_state != Actions::NONE) {
            auto time = SDL_GetTicks();
            if (time - previous_timestamp > repetition_timer_ms) { // Long press timer elapsed
                previous_timestamp = time;
                previous_button_state = current_button_state; // Consume button press
            } else {
                auto b = current_button_state;
                current_button_state = current_button_state & (~previous_button_state); // Filter out previous button presses
                previous_button_state = b; // Add current buttons to previous buttons
            }
        } else {
            previous_button_state = Actions::NONE;
            previous_timestamp = SDL_GetTicks();
        }
        return current_button_state;
    }

    int singleTriggerActions(int& previous_button_state, int current_button_state) {
        auto ret = current_button_state & ~previous_button_state;
        previous_button_state = current_button_state;
        return ret;
    }

    int getActions() {
        int button_state = Actions::NONE;
        if (any_key_pressed()) {
            if (isKeyPressed(KEY_NSPIRE_N)) button_state |= Actions::SPAWN_NEXT_WAVE;
            if (isKeyPressed(KEY_NSPIRE_ESC)) button_state |= Actions::ESC;
            if (isKeyPressed(KEY_NSPIRE_LEFT) || isKeyPressed(KEY_NSPIRE_4)) button_state |= Actions::LEFT;
            if (isKeyPressed(KEY_NSPIRE_RIGHT) || isKeyPressed(KEY_NSPIRE_6)) button_state |= Actions::RIGHT;
            if (isKeyPressed(KEY_NSPIRE_UP) || isKeyPressed(KEY_NSPIRE_8)) button_state |= Actions::UP;
            if (isKeyPressed(KEY_NSPIRE_DOWN) || isKeyPressed(KEY_NSPIRE_2)) button_state |= Actions::DOWN;
            if (isKeyPressed(KEY_NSPIRE_ENTER) || isKeyPressed(KEY_NSPIRE_SPACE) || isKeyPressed(KEY_NSPIRE_5)) button_state |= Actions::CONFIRM;
            if (isKeyPressed(KEY_NSPIRE_P)) button_state |= Actions::PAUSE;
            if (isKeyPressed(KEY_NSPIRE_S)) button_state |= Actions::SHOP;
        }
        return button_state;
    }
}
