#include "state_machine.hpp"


void State::addTransition(RReader<State> target_state, Input::Actions when) {
    transitions_[when] = target_state;
}

RReader<State> State::parseTrnsitionAction(int single_trigger_actions) {
    for (const auto& [k, v] : transitions_) {
        if (k & single_trigger_actions) {
            return v;
        }
    }
    return RReader<State>{};
}

///////////////////////////////////////////////////////////////////////////////

void StateMachine::tick() {
    current_state_->tick();
}

bool StateMachine::parseTrnsitionAction(int single_trigger_actions) {
    RReader<State> new_state = current_state_->parseTrnsitionAction(single_trigger_actions);
    if (new_state) {
        current_state_->onExit();
        current_state_ = new_state;
        new_state->onEnter();
        return true;
    }
    return false;
}

RReader<State> StateMachine::addState(ROwner<State>&& state) {
    #ifdef DEBUG
    if (std::find(states_.begin(), states_.end(), state) != states_.end()) {
        throw "State already in state machine.";
    }
    #endif

    auto ret = state.makeReader();
    states_.push_back(std::move(state));
    return ret;
}

void StateMachine::addTransition(RReader<State> from, RReader<State> to, Input::Actions when) {
    from->addTransition(to, when);
}

