#include "main_sm.hpp"
#include "shop_sm.hpp"
#include "wave_sm.hpp"
#include <SDL/SDL_timer.h>
#include "../util/constants.hpp"
#include "../game_manager.hpp"
#include "../util/macros.hpp"

bool on_shop_input(Event& e){
    if (e.getType() == Event::USER_INPUT) {
        auto& uie = static_cast<UserInputEvent&>(e);
        bool ret = (bool)(uie.getPositiveEdge(Input::Actions::SHOP));
        if (ret) uie.accept();
        return ret;
    }
    return false;
};

MainSM::MainSM(RReader<GameManager> manager)
    : StateMachine{}
    , manager_{manager}
    , input_handler_{3 * STANDARD_TICK_DURATION}
 {
    wave_sm_ = static_cast<RReader<WaveSM>>(addState(ROwner<State>(new WaveSM(manager))));
    shop_sm_ = static_cast<RReader<ShopSM>>(addState(ROwner<State>(new ShopSM(manager))));

    addTransition(shop_sm_, wave_sm_, on_shop_input);
    addTransition(wave_sm_, shop_sm_, on_shop_input);

    setInitialState(wave_sm_);
}

void MainSM::start() {
    StateMachine::start();
    wave_sm_->start();
    shop_sm_->start();
}


const Transition* MainSM::parseEvent(Event& event) {
    LOG("Parsing\n");
    if (event.getType() == Event::Type::USER_INPUT) {
        auto& uie = static_cast<UserInputEvent&>(event);
        if (uie.getRaw(Input::Actions::ESC)) {
            event.accept();
            stop();
            return nullptr;
        }
    }

    return StateMachine::parseEvent(event);
}


void MainSM::tick() {
    Uint32 start = SDL_GetTicks();
    input_handler_.tick();
    if (input_handler_.hasEvent()) {
        auto event = input_handler_.getEvent();
        parseEvent(event);
    }
    current_state_->tick();

    Uint32 stop = SDL_GetTicks();
    Uint32 frame_time = stop - start;
    if (frame_time < STANDARD_TICK_DURATION) {
        SDL_Delay(STANDARD_TICK_DURATION - frame_time);
    }
}
