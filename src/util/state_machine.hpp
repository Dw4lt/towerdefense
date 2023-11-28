#pragma once
#include <map>
#include <vector>
#include <functional>
#include "../input.hpp"
#include "../util/ownership.hpp"
#include "event.hpp"


struct State;
struct StateMachine;


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
    friend class StateMachine;
    friend class EndState;

    State() = default;

    /// @brief Method called once upon entering of state
    virtual void onEnter() {};

    /// @brief Method called once prior to leaving state
    virtual void onExit() {};

    /// @brief Method called once per tick while in state
    virtual void tick() {};

    virtual ~State() = default;

    /// @brief Add transition leading out of state
    virtual void addTransition(Transition&& transition) { transitions_.emplace_back(std::move(transition)); };

    /// @brief Check for valid transitions given a certain action
    /// @param event Event to process
    /// @return Desired transition, should the condition match, else nullptr
    virtual const Transition* processEvent(Event& event);

    /// @brief Raise an event to the parent state machine
    void bubbleUpEvent(ROwner<Event>&& event);

protected:


private:

    std::vector<Transition> transitions_;

    /// @brief Parent state machine, set by the state machine.
    StateMachine* parent_ = nullptr;
};

/// @brief Resumable, restartable state machine which can also behave as a state
struct StateMachine : public State {
    friend class State;
    friend class EndState;

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
    void stop() noexcept;

    /// @brief Whether or not the state machine was marked as stopped. May be restarted from initial state.
    bool stopped() const noexcept;

    /// @brief Initialize state machine with a starting state. State must already be known.
    /// @param initial_state State to start with
    virtual void setInitialState(RReader<State> initial_state);

    /// @brief Let current state check for valid transitions given a certain action
    /// @param event Event to process
    /// @return Desired transition, should the condition match, else nullptr
    virtual const Transition* processEvent(Event& event) override;


    /// @brief Create a new, unconnected state and add it to the state machine
    /// @tparam ExtendsState Type of state to create and add
    /// @param __args Arguments passed to constructor of state
    /// @return RReader of the new state
    template<typename ExtendsState, typename... Args,
        typename = std::enable_if_t<
            std::conjunction_v<
                std::is_base_of<State, ExtendsState>,
                std::is_constructible<ExtendsState, Args...>
            >
        >
    >
    RReader<ExtendsState> addState(Args&&... __args) {
        states_.emplace_back(new ExtendsState(std::forward<Args>(__args)...));
        auto& state = states_.back();
        state->parent_ = this;
        return static_cast<RReader<ExtendsState>>(state.makeReader());
    };

    /// @brief Add a transition from one state to another based on a certain event
    void addTransition(RReader<State> from, RReader<State> to, Condition condition, bool skip_exit = false, bool skip_entry = false);

    bool hasPendingChildEvents() {
        return child_events_.size() > 0;
    }

    /// @brief Get the oldest event in the queue, remove it from the queue
    ROwner<Event> popChildEvent() {
        ROwner<Event> helper(nullptr);
        std::swap(helper, child_events_.front());
        child_events_.pop();
        return helper;
    }

protected:

    /// @brief Template for a simple event check
    /// @tparam type Event::Type to check for before casting
    /// @tparam EventClass Class of event to cast to
    /// @param check Lambda in charge of the final decision
    /// @return Condition which can be used as part of a transition
    template<typename EventClass, Event::Type type>
    static Condition makeEventChecker(std::function<bool(EventClass&)> check) {
        return [check](Event& event) {
            if (event.getType() == type) {
                auto& e = static_cast<EventClass&>(event);
                if (check(e)) {
                    e.accept();
                    return true;
                }
            }
            return false;
        };
    };

    /// @brief Make condition which accepts if the event is a UserInputEvent and matches the given actions matches the single-trigger actions
    static Condition makeOnSingleFireActionCondition(int actions);

    /// @brief Make condition which accepts if the event is a GameUpdateEvent and matches the given flag
    static Condition makeGameEventCondition(GameUpdateEvent::UpdateType type);

    /// @brief First state to be entered upon starting state machine
    RReader<State> initial_state_;

    /// @brief Current state of the state machine
    RReader<State> current_state_;

private:
    /// @brief Receive a new event from a child state
    void receiveChildEvent(ROwner<Event>&& event) {
        child_events_.push(std::move(event));
    };

    /// @brief Handle a given transition, if possible. Else return it.
    const Transition* handleTransition(Event& event, const Transition* transition);

    /// @brief Check whether the given state is part of this state machine.
    bool isInternalState(const RReader<State>& state);

    /// @brief States owned by this state machine
    std::vector<ROwner<State>> states_;

    /// @brief Queue of events emitted by child states/machines
    std::queue<ROwner<Event>> child_events_;

    bool stopped_ = false;
};


/// @brief End-state. Entering this state stops the state machine automatically.
struct EndState final : public State {

    /// @brief Override to disable exiting this state via transitions
    virtual void addTransition(Transition&&) override {
        throw "End state is final.";
    };

    /// @brief Stop parent state machine
    virtual void onEnter() override;
};
