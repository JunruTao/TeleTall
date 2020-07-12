#ifndef TELEPAD_H
#define TELEPAD_H

#include <SDL2/SDL.h>
#include "teletall_control.h"
#include "teletall_cursor.h"
#include "JUTA_math.h"

class Telepad
{
public:
    Telepad(
        const size_t &master_width,
        const size_t &master_height);
    ~Telepad();

    void Update(const Telecontroller &controller);
    void Render(SDL_Renderer *renderer);

private:
    size_t win_width;
    size_t win_height;
    size_t pad_width;
    size_t pad_height;
    SDL_Rect padViewport;

    SDL_Rect slidebar;
    bool slidebarSelected;
};

#endif