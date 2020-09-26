#include "wood.hpp"
#include <assert.h>

bool Wood::initialized_{false};
Wood Wood::instance_{};

Wood& Wood::instance(){
    assert("Instance must be initualized before use!" && initialized_);
    return Wood::instance_;
}

void Wood::init(unsigned long start_amount){
    if (!initialized_){
        Wood::instance_ = Wood(start_amount);
        initialized_ = true;
    }
}

Wood::Wood(unsigned long start_amount)
    : Resource(start_amount,
                "Wood",
                "Used for building and energy production. Can be harvested from forests.",
                "Wd",
                "W")
{

}

Wood::~Wood() {

}
