#ifndef RESOURCE_H
#define RESOURCE_H
#include <SDL/SDL_video.h>
#include <string>

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
    std::string nameShort() const;
    std::string nameSymbol() const;
    std::string description() const;

protected:
    unsigned long stock_;

    std::string name_full_;
    std::string name_short_;
    std::string name_symbol_;
    std::string description_;
};

#endif
