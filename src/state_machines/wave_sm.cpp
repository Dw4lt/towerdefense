
#include "wave_sm.hpp"
#include "../game_state.hpp"
#include "../game_manager.hpp"
#include "../util/macros.hpp"

BaseWaveS::BaseWaveS(RReader<GameManager> manager)
    : State{}
    , manager_{manager}
{}

const Transition* BaseWaveS::parseEvent(Event& event) {
    if (event.getType() == Event::Type::USER_INPUT) {
        auto& uie = static_cast<UserInputEvent&>(event);
        manager_->applyUserInputToFieldCursor(uie);
    }
    return State::parseEvent(event); // Check and apply potential transition event
}

///////////////////////////////////////////////////////////////////////////////

void OngoingWaveS::onEnter() {
    manager_->spawnWave();
}

void OngoingWaveS::onExit() {
    manager_->endWave(); // TODO: only if survived and all enemies are dead
}

void OngoingWaveS::tick() {
    manager_->mainGameLoop();
    manager_->mainRenderLoop();
}

///////////////////////////////////////////////////////////////////////////////

void OngoingWavePausedS::tick() {
    manager_->mainRenderLoop();
}


///////////////////////////////////////////////////////////////////////////////

void BetweenWavesS::tick() {
    manager_->mainRenderLoop();
}

///////////////////////////////////////////////////////////////////////////////

bool onPauseButtonPressed(Event& e) { // TODO: capture keys via curry lamda and make this a general utility function?
    if (e.getType() == Event::Type::USER_INPUT) {
        auto& uie = static_cast<UserInputEvent&>(e);
        if (uie.getPositiveEdge(Input::PAUSE)) {
            uie.accept();
            return true;
        }
    }
    return false;
}

WaveSM::WaveSM(RReader<GameManager> manager)
    : StateMachine{}
    , manager_{manager}
{
    auto between_waves_state = addState(ROwner<State>(new BetweenWavesS(manager)));
    auto ongoing_wave_state = addState(ROwner<State>(new OngoingWaveS(manager)));
    auto ongoing_wave_paused_state = addState(ROwner<State>(new OngoingWavePausedS(manager)));

    addTransition(between_waves_state, ongoing_wave_state, [](Event& e) {
        if (e.getType() == Event::Type::USER_INPUT) {
            auto& uie = static_cast<UserInputEvent&>(e);
            if (uie.getPositiveEdge(Input::SPAWN_NEXT_WAVE)) {
                uie.accept();
                return true;
            }
        }
        return false;
    });

    addTransition(ongoing_wave_state, ongoing_wave_paused_state, onPauseButtonPressed, true, false); // *Do not pass start*
    addTransition(ongoing_wave_paused_state, ongoing_wave_state, onPauseButtonPressed, false, true); // Do not spawn new wave
    // addTransition(ongoing_wave_state, between_waves_state, /*onWaveDefeated*/); // TODO: end of wave here

    setInitialState(between_waves_state);
}
