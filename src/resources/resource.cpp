#include "resource.hpp"

Resource::Resource(unsigned long start_amount,
                   std::string full_name,
                   std::string description,
                   std::string short_name,
                   std::string name_symbol)
    : stock_(start_amount)
    , name_full_(full_name)
    , name_short_(short_name)
    , name_symbol_(name_symbol)
    , description_(description) {
}

Resource::~Resource() {
}

unsigned long Resource::stock() const {
    return stock_;
}

void Resource::add(unsigned long amount) {
    stock_ += amount;
}

void Resource::subtract(unsigned long amount) {
    if (amount > stock_) {
        amount = stock_;
    }
    stock_ -= amount;
}

std::string Resource::name() const {
    return name_full_;
}

std::string Resource::nameShort() const {
    return name_short_;
}

std::string Resource::nameSymbol() const {
    return name_symbol_;
}

std::string Resource::description() const {
    return description_;
}
