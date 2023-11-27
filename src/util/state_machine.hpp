#pragma once
#include <map>
#include <vector>
#include <functional>
#include "../input.hpp"
#include "../util/ownership.hpp"
#include "event.hpp"


struct State;


using Condition = std::function<bool(Event&)>;

struct Transition {
    /// @brief Source state to jump from
    RReader<State> from;

    /// @brief Target state to jumpt to should the condition be met
    RReader<State> to;

    /// @brief Condition to check upon event
    Condition condition;

    bool skip_exit;
    bool skip_entry;
};

struct State {
    /// @brief Method called once upon entering of state
    virtual void onEnter() {};

    /// @brief Method called once prior to leaving state
    virtual void onExit() {};

    /// @brief Method called once per tick while in state
    virtual void tick() {};

    virtual ~State() = default;

    /// @brief Add transition leading out of state
    void addTransition(Transition&& transition) { transitions_.emplace_back(std::move(transition)); };

    /// @brief Check for valid transitions given a certain action
    /// @param event Event to process
    /// @return Desired transition, should the condition match, else nullptr
    virtual const Transition* parseEvent(Event& event);

private:
    std::vector<Transition> transitions_;
};

struct StateMachine : public State {
    StateMachine() = default;

    virtual ~StateMachine() = default;

    /// @brief Tick the current state
    virtual void tick();

    /// @brief Method called upon state machine resume
    virtual void onEnter();

    /// @brief Method called before state machine is halted (may be resumed again)
    virtual void onExit();

    /// @brief Start the state machine, trigger the transition to the initial state
    virtual void start();

    /// @brief Mark the state machine as stopped
    void stop();

    /// @brief Whether or not the state machine was marked as stopped. May be restarted from initial state.
    bool stopped();

    /// @brief Initialize state machine with a starting state. State must already be known.
    /// @param initial_state State to start with
    virtual void setInitialState(RReader<State> initial_state);

    /// @brief Let current state check for valid transitions given a certain action
    /// @param event Event to process
    /// @return Desired transition, should the condition match, else nullptr
    virtual const Transition* parseEvent(Event& event) override;

    /// @brief Add a state to the state machine (unconnected)
    RReader<State> addState(ROwner<State>&& state);

    /// @brief Add a transition from one state to another based on a certain event
    void addTransition(RReader<State> from, RReader<State> to, Condition&& condition, bool skip_exit = false, bool skip_entry = false);

    void addTransition(Transition&& transition);

protected:
    /// @brief First state to be entered upon starting state machine
    RReader<State> initial_state_;

    /// @brief Current state of the state machine
    RReader<State> current_state_;

private:

    /// @brief Handle a given transition, if possible. Else return it.
    const Transition* handleTransition(Event& event, const Transition* transition);

    /// @brief Check whether the given state is part of this state machine.
    bool isInternalState(const RReader<State>& state);

    /// @brief States owned by this state machine
    std::vector<ROwner<State>> states_;

    bool stopped_ = false;
};
