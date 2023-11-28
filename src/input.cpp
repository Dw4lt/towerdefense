#include "input.hpp"
#include "SDL/SDL_timer.h"
#include "util/event.hpp"


Input::Input(int delay_between_repeats)
    : prior_actions{0}
    , prior_repeat_actions{0}
    , prior_timestamp{SDL_GetTicks()}
    , delay_between_repeats_{delay_between_repeats}
    , input_event_queue_{}
{
}

void Input::tick() {
    int input = getActions();

    if (input) {
        int single_trigger_inputs = Input::singleTriggerActions(prior_actions, input);

        Uint32 current_timestamp = SDL_GetTicks();
        int repeat_input = Input::actionRepetitionOnLongPress(prior_timestamp, current_timestamp, prior_repeat_actions, input, delay_between_repeats_);

        auto&& event = UserInputEvent(input, repeat_input, single_trigger_inputs);
        input_event_queue_.push(event);
    }
    prior_actions = input;
}

UserInputEvent Input::getEvent() {
    auto ret = input_event_queue_.front();
    input_event_queue_.pop();
    return ret;
}

int Input::actionRepetitionOnLongPress(Uint32& previous_timestamp, Uint32 current_time_stamp, int& previous_button_state, int current_button_state, Uint32 repetition_timer_ms) {
    if (current_button_state != Actions::NONE) {
        if (current_time_stamp - previous_timestamp > repetition_timer_ms) { // Long press timer elapsed
            previous_timestamp = current_time_stamp;
            previous_button_state = current_button_state; // Consume button press
        } else {
            auto b = current_button_state;
            current_button_state = current_button_state & (~previous_button_state); // Filter out previous button presses
            previous_button_state = b; // Add current buttons to previous buttons
        }
    } else {
        previous_button_state = Actions::NONE;
        previous_timestamp = current_time_stamp;
    }
    return current_button_state;
}

int Input::singleTriggerActions(int previous_button_state, int current_button_state) {
    auto ret = current_button_state & ~previous_button_state;
    previous_button_state = current_button_state;
    return ret;
}

int Input::getActions() {
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
        if (isKeyPressed(KEY_NSPIRE_R)) button_state |= Actions::RESTART;
    }
    return button_state;
}
