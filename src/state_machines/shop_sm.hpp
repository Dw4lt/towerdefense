#pragma once
#include "../util/state_machine.hpp"

class GameManager;

struct TowerSelectionShopS : public State {
    TowerSelectionShopS(RReader<GameManager> manager);

    virtual void tick();

private:
    RReader<GameManager> manager_;
};


struct ShopSM : public StateMachine {
    ShopSM(RReader<GameManager> manager);

    virtual void onEnter();

    virtual void onExit();

    virtual void tick();

private:
    RReader<GameManager> manager_;
};
