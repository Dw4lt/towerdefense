#include "state_machine.hpp"
#include "macros.hpp"


const Transition* State::parseEvent(Event& event) {
    for (const auto& t : transitions_) {
        if (t.condition(event)) {
            event.accept();
            return &t;
        }
    }
    return nullptr;
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

bool StateMachine::stopped() {
    return stopped_;
}

void StateMachine::stop() {
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

const Transition* StateMachine::parseEvent(Event& event) {
    const Transition* transition = State::parseEvent(event);
    if (transition) { // Hit when this state machine is used as a state as well (eg. as a sub-state machine)
        return handleTransition(event, transition);
    }

    transition = current_state_->parseEvent(event);
    if (transition) { // Event triggered a transition in a sub-state
        return handleTransition(event, transition);
    }

    return nullptr; // No state change occurred.
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

void StateMachine::addTransition(RReader<State> from, RReader<State> to, Condition&& condition, bool skip_exit, bool skip_entry) {
    if (from == to) {
        throw "Attempted to add circular state transition.";
    }
    from->addTransition(Transition{from: from, to: to, condition: std::move(condition), skip_exit: skip_exit, skip_entry: skip_entry});
}

void StateMachine::addTransition(Transition&& transition) {
    transition.from->addTransition(std::move(transition));
}

