#include "main_sm.hpp"
#include "shop_sm.hpp"
#include "wave_sm.hpp"
#include <SDL/SDL_timer.h>
#include "../util/constants.hpp"
#include "../game_manager.hpp"
#include "../util/macros.hpp"


MainSM::MainSM(RReader<GameManager> manager)
    : StateMachine{}
    , manager_{manager}
    , input_handler_{3 * STANDARD_TICK_DURATION}
 {
    wave_sm_ = addState<WaveSM>(manager);
    shop_sm_ = addState<ShopSM>(manager);

    auto onExitGame = makeOnSingleFireActionCondition(Input::ESC);
    auto onShopEnter = makeOnSingleFireActionCondition(Input::SHOP);
    auto onShopExit = makeOnSingleFireActionCondition(Input::SHOP | Input::ESC); // TODO: a menu would be nice.

    addTransition(wave_sm_, shop_sm_, onShopEnter);
    addTransition(shop_sm_, wave_sm_, onShopExit);

    addTransition(wave_sm_, addState<EndState>(), onExitGame);

    setInitialState(wave_sm_);
}

void MainSM::start() {
    StateMachine::start();
    wave_sm_->start();
    shop_sm_->start();
}

void MainSM::tick() {
    Uint32 start = SDL_GetTicks();
    input_handler_.tick();
    if (input_handler_.hasEvent()) {
        auto event = input_handler_.getEvent();
        processEvent(event);
    }
    current_state_->tick();

    Uint32 stop = SDL_GetTicks();
    Uint32 frame_time = stop - start;
    if (frame_time < STANDARD_TICK_DURATION) {
        SDL_Delay(STANDARD_TICK_DURATION - frame_time);
    }
}
