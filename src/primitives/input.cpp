#include "input.hpp"

namespace Input {
    int getActions() {
        int button_state = Actions::NONE;
        if (any_key_pressed()) {
            if (isKeyPressed(KEY_NSPIRE_N)) button_state |= Actions::SPAWN_NEXT_WAVE;
            if (isKeyPressed(KEY_NSPIRE_ESC)) button_state |= Actions::ESC;
            if (isKeyPressed(KEY_NSPIRE_LEFT) || isKeyPressed(KEY_NSPIRE_4)) button_state |= Actions::LEFT;
            if (isKeyPressed(KEY_NSPIRE_RIGHT) || isKeyPressed(KEY_NSPIRE_6)) button_state |= Actions::RIGHT;
            if (isKeyPressed(KEY_NSPIRE_UP) || isKeyPressed(KEY_NSPIRE_8)) button_state |= Actions::UP;
            if (isKeyPressed(KEY_NSPIRE_DOWN) || isKeyPressed(KEY_NSPIRE_2)) button_state |= Actions::DOWN;
            if (isKeyPressed(KEY_NSPIRE_ENTER) || isKeyPressed(KEY_NSPIRE_SPACE)) button_state |= Actions::CONFIRM;
        }
        return button_state;
    }
}
