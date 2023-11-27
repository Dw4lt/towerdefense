#include "shop_sm.hpp"
#include "../game_manager.hpp"


TowerSelectionShopS::TowerSelectionShopS(RReader<GameManager> manager)
    : State{}
    , manager_{manager}
{}

void TowerSelectionShopS::tick() {
    manager_->shopRenderLoop();
    // TODO: shop logic
};

///////////////////////////////////////////////////////////////////////////////

ShopSM::ShopSM(RReader<GameManager> manager)
    : StateMachine{}
    , manager_{manager}
{
    auto tower_selection_state = addState(ROwner<State>(new TowerSelectionShopS(manager)));
    setInitialState(tower_selection_state);
}

void ShopSM::onEnter() {
    manager_->setShopVisibility(true);
}

void ShopSM::onExit() {
    manager_->setShopVisibility(false);
}

void ShopSM::tick() {
    StateMachine::tick();
}
