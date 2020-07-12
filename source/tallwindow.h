#ifndef TALLWINDOW_H
#define TALLWINDOW_H

#include <SDL2/SDL.h>
#include "teletall_control.h"

class Tallwindow
{
public:
    Tallwindow(
        const size_t &master_width,
        const size_t &master_height);
    ~Tallwindow();

    void Update(const Telecontroller &controller);

    void Render(SDL_Renderer *renderer);

private:
    size_t tlwd_Width;
    size_t tlwd_Height;
    size_t start_pt;

    SDL_Rect tallViewport;
};

#endif