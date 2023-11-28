
#include "wave_sm.hpp"
#include "../game_state.hpp"
#include "../game_manager.hpp"
#include "../util/macros.hpp"

BaseWaveS::BaseWaveS(RReader<GameManager> manager)
    : State{}
    , manager_{manager}
{}

const Transition* BaseWaveS::processEvent(Event& event) {
    if (event.getType() == Event::Type::USER_INPUT) {
        auto& uie = static_cast<UserInputEvent&>(event);
        manager_->applyUserInputToFieldCursor(uie);
    }
    return State::processEvent(event); // Check and apply potential transition event
}

///////////////////////////////////////////////////////////////////////////////

void OngoingWaveS::onEnter() {
    manager_->spawnWave();
}

void OngoingWaveS::onExit() {
    manager_->endWave();
}

void OngoingWaveS::tick() {
    manager_->mainGameLoop();
    manager_->mainRenderLoop();
    auto state = GameState::getState();
    if (state->getLives() < 1) bubbleUpEvent(ROwner<Event>(new GameUpdateEvent(GameUpdateEvent::UpdateType::GAME_LOST)));
    else if (!state->anyEnemiesPresent()) bubbleUpEvent(ROwner<Event>(new GameUpdateEvent(GameUpdateEvent::UpdateType::WAVE_ENDED)));
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

void LostWaveS::onEnter() {
}

void LostWaveS::onExit() {
    manager_->reset();
}

void LostWaveS::tick() {
    manager_->lostGameLoop();
    manager_->lostRenderLoop();
    // TODO: show overlay
}

///////////////////////////////////////////////////////////////////////////////

void WaveSM::tick() {
    StateMachine::tick();
    while (hasPendingChildEvents()) {
        auto event = popChildEvent();
        StateMachine::processEvent(*event);
        if (!event->isAccepted()) {
            bubbleUpEvent(std::move(event));
        }
    }
}

WaveSM::WaveSM(RReader<GameManager> manager)
    : StateMachine{}
    , manager_{manager}
{
    auto between_waves_state = addState<BetweenWavesS>(manager);
    auto ongoing_wave_state = addState<OngoingWaveS>(manager);
    auto ongoing_wave_paused_state = addState<OngoingWavePausedS>(manager);
    auto game_lost_state = addState<LostWaveS>(manager);

    auto onNextWavePressed = makeOnSingleFireActionCondition(Input::SPAWN_NEXT_WAVE);
    auto onPauseButtonPressed = makeOnSingleFireActionCondition(Input::PAUSE);
    auto onRestartButtonPressed = makeOnSingleFireActionCondition(Input::RESTART);
    auto onWaveEnd = makeGameEventCondition(GameUpdateEvent::UpdateType::WAVE_ENDED);
    auto onGameLost = makeGameEventCondition(GameUpdateEvent::UpdateType::GAME_LOST);

    addTransition(between_waves_state, ongoing_wave_state, onNextWavePressed);

    addTransition(ongoing_wave_state, ongoing_wave_paused_state, onPauseButtonPressed, true, false); // *Do not pass start*
    addTransition(ongoing_wave_paused_state, ongoing_wave_state, onPauseButtonPressed, false, true); // Do not spawn new wave
    addTransition(ongoing_wave_state, between_waves_state, onWaveEnd);

    addTransition(ongoing_wave_state, game_lost_state, onGameLost, true, false);
    addTransition(game_lost_state, between_waves_state, onRestartButtonPressed);

    setInitialState(between_waves_state);
}
