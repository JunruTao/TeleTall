#ifndef TELEPAD_H
#define TELEPAD_H

#include <SDL2/SDL.h>
#include "teletall_control.h"

class Telepad
{
public:
    Telepad(
        const size_t& master_width, 
        const size_t& master_height, 
        const Telecontroller& controller);
    ~Telepad();

private:
    size_t tlpd_Width;
    size_t tlpd_Height;
};

#endif