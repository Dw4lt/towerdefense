#include "state_machine.hpp"
#include "macros.hpp"


const Transition* State::processEvent(Event& event) {
    for (const auto& t : transitions_) {
        if (t.condition(event)) {
            event.accept();
            return &t;
        }
    }
    return nullptr;
}

void State::bubbleUpEvent(ROwner<Event>&& event) {
    if (parent_) parent_->receiveChildEvent(std::move(event));
}

///////////////////////////////////////////////////////////////////////////////

void StateMachine::tick() {
    current_state_->tick();
}

void StateMachine::start() {
    if (!initial_state_) {
        throw "Attempted to start a state machine without a declared initial state.";
    }
    current_state_ = initial_state_;
    stopped_ = false;
    initial_state_->onEnter();
}

void StateMachine::onEnter() {}

void StateMachine::onExit() {}

bool StateMachine::stopped() const noexcept {
    return stopped_;
}

void StateMachine::stop() noexcept {
    stopped_ = true;
}

void StateMachine::setInitialState(RReader<State> initial_state) {
    #ifdef DEBUG
    if (!isInternalState(initial_state)) {
        throw "State not known to state machine.";
    }
    #endif
    initial_state_ = initial_state;
}

bool StateMachine::isInternalState(const RReader<State>& state) {
    const State* ptr = state.get();
    for (auto& s : states_) {
        if (s.get() == ptr) {
            return true;
        }
    }
    return false;
}

const Transition* StateMachine::handleTransition(Event& event, const Transition* transition) {
    auto& t = *transition;
    if (isInternalState(t.to)) {
        if (!t.skip_exit) current_state_->onExit();
        current_state_ = t.to;
        if (!t.skip_entry) current_state_->onEnter();
        event.accept();
        return nullptr;
    } else {
        // Intra-state-machine transition
        return transition;
    }
}

const Transition* StateMachine::processEvent(Event& event) {
    const Transition* transition = State::processEvent(event);
    if (transition) { // Hit when this state machine is used as a state as well (eg. as a sub-state machine)
        return handleTransition(event, transition);
    }

    transition = current_state_->processEvent(event);
    if (transition) { // Event triggered a transition in a sub-state
        return handleTransition(event, transition);
    }

    return nullptr; // No state change occurred.
}

void StateMachine::addTransition(RReader<State> from, RReader<State> to, Condition condition, bool skip_exit, bool skip_entry) {
    if (from == to) {
        throw "Attempted to add circular state transition.";
    }
    static_cast<RReader<State>>(from)->addTransition(Transition{from: from, to: to, condition: std::move(condition), skip_exit: skip_exit, skip_entry: skip_entry});
}

Condition StateMachine::makeOnSingleFireActionCondition(int actions) {
    return makeEventChecker<UserInputEvent, Event::Type::USER_INPUT> (
        [actions](UserInputEvent& event){
            return event.getPositiveEdge(actions);
        }
    );
};

Condition StateMachine::makeGameEventCondition(GameUpdateEvent::UpdateType type) {
    return makeEventChecker<GameUpdateEvent, Event::Type::GAME_UPDATE> (
        [type](GameUpdateEvent& event){
            return type == event.getUpdateType();
        }
    );
};

///////////////////////////////////////////////////////////////////////////////

void EndState::onEnter() {
    parent_->stop();
};
