#pragma once

struct Event {

    enum Type {
        USER_INPUT,
        NONE,
    };

    Event(Type type = NONE);

    virtual ~Event() = default;

    Type getType();

    /// @brief Accept event, marking it as handled.
    void accept() { is_accepted_ = true; };

    /// @brief Whether the event was accepted
    bool isAccepted() { return is_accepted_; };

private:
    /// @brief Whether the event was accepted
    bool is_accepted_ = false;

    Type type_;
};


struct UserInputEvent : public Event {
    /// @brief
    /// @param input The raw, unmodified, combined input state
    /// @param autorepeat_inputs Inputs with auto-repeat and delay between button registrations
    /// @param positive_edge_inputs Inputs that are newly pressed. Keeping a button pressed only results in it registering once.
    UserInputEvent(int input, int autorepeat_inputs, int positive_edge_inputs);

    /// @brief Mark given inputs as handled. Should no outstanding inputs be left, the entire event is accepted.
    void partialAccept(int mask);

    /// @brief Get raw input state
    /// @param mask Bitmask of interest
    /// @return Raw input state masked by mask
    int getRaw(int mask = __INT_MAX__);

    /// @brief Inputs with auto-repeat and delay between button registrations
    /// @param mask Bitmask of interest
    /// @return Input state with repeat, masked by mask
    int getAutorepeat(int mask = __INT_MAX__);

    /// @brief Get nputs that are newly pressed
    /// @param mask Bitmask of interest
    /// @return Newly presset input state masked by mask
    int getPositiveEdge(int mask = __INT_MAX__);

protected:
    int input_;
    int autorepeat_inputs_;
    int positive_edge_inputs_;
};
