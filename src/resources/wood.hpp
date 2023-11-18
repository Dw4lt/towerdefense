#pragma once
#include "resource.hpp"

class Wood : public Resource {
public:
    static Wood& instance();
    static void init(unsigned long start_amount = 0);
    virtual ~Wood();

private:
    Wood(unsigned long start_amount = 0);
    static bool initialized_;
    static Wood instance_;
};
