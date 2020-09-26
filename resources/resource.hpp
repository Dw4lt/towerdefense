#ifndef RESOURCE
#define RESOURCE
#include <SDL/SDL.h>
#include<string>

class Resource {
public:
    Resource(unsigned long start_amount,
             std::string full_name,
             std::string description,
             std::string short_name, 
             std::string name_symbol);
    virtual ~Resource();

    void add(unsigned long amount);
    void multiply(double factor);
    void subtract(unsigned long amount);
    unsigned long stock() const;

    std::string name() const;
    std::string name_short() const;
    std::string name_symbol() const;
    std::string description() const;

protected:
    unsigned long stock_;

    std::string name_full_;
    std::string name_short_;
    std::string name_symbol_;
    std::string description_;

};

#endif