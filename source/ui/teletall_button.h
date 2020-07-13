#ifndef TELETALL_BUTTON_H
#define TELETALL_BUTTON_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class Button
{
public:
    Button();

    std::string GetName() const;
    std::string GetInfo() const;
    std::string GetID() const;

private:
    std::string _name;
    std::string _id;
    std::string _info;

    static size_t _count;
};

#endif