#include "../util/state_machine.hpp"
#include "../util/ownership.hpp"
#include "../input.hpp"

class GameManager;
class WaveSM;
class ShopSM;

struct MainSM : public StateMachine {
    MainSM(RReader<GameManager> manager);

    void tick();

    virtual void start() override;

private:
    RReader<GameManager> manager_;
    Input input_handler_;

    RReader<WaveSM> wave_sm_;
    RReader<ShopSM> shop_sm_;
};
