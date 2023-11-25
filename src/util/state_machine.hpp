#pragma once
#include <map>
#include <vector>
#include "../input.hpp"
#include "../util/ownership.hpp"

struct State {
    /// @brief Method called once upon entering of state
    virtual void onEnter() {};

    /// @brief Method called once prior to leaving state
    virtual void onExit() {};

    /// @brief Method called once per tick while in state
    virtual void tick() {};

    virtual ~State() = default;

    /// @brief Add transition leading out of state
    /// @param target_state State to transition to
    /// @param when Action required for transition
    void addTransition(RReader<State> target_state, Input::Actions when);

    /// @brief Check for valid transitions given a certain action
    /// @param single_trigger_actions Actions to check for (after positive edge filter)
    /// @return Whether a transition was found
    RReader<State> parseTrnsitionAction(int single_trigger_actions);

private:
    std::map<Input::Actions, RReader<State>> transitions_;
};

struct StateMachine {
    /// @brief Tick the current state
    virtual void tick();

    /// @brief Let current state check for valid transitions given a certain action
    /// @param single_trigger_actions Actions to check for (after positive edge filter)
    /// @return Whether a transition was found
    bool parseTrnsitionAction(int single_trigger_actions);

    /// @brief Add a state to the state machine (unconnected)
    RReader<State> addState(ROwner<State>&& state);

    /// @brief Add a transition from one state to another based on a certain action
    void addTransition(RReader<State> from, RReader<State> to, Input::Actions when);

private:
    /// @brief States owned by this state machine
    std::vector<ROwner<State>> states_;

    /// @brief Current state of the state machine
    RReader<State> current_state_;
};
