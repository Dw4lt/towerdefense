#include "event.hpp"


Event::Event(Type type)
    : type_{type}
{

}

Event::Type Event::getType() const noexcept {
    return type_;
}

///////////////////////////////////////////////////////////////////////////////

UserInputEvent::UserInputEvent(int input, int autorepeat_inputs, int positive_edge_inputs)
    : Event(Event::Type::USER_INPUT)
    , input_{input}
    , autorepeat_inputs_{autorepeat_inputs}
    , positive_edge_inputs_{positive_edge_inputs}
{

}

void UserInputEvent::partialAccept(int mask) {
    int negative_mask = ~mask;
    input_ &= negative_mask;
    autorepeat_inputs_ &= negative_mask;
    positive_edge_inputs_ &= negative_mask;

    if (!input_) {
        accept();
    }
}

int UserInputEvent::getRaw(int mask) {
    return input_ & mask;
}

int UserInputEvent::getAutorepeat(int mask) {
    return autorepeat_inputs_ & mask;
}

int UserInputEvent::getPositiveEdge(int mask) {
    return positive_edge_inputs_ & mask;
}

