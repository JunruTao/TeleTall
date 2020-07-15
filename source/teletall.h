#ifndef TELETALL_H
#define TELETALL_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include "telepad.h"
#include "tallwindow.h"
#include "teletall_control.h"
#include "ui/teletall_graphics.h"

class TeleTall
{
public:
    //Constructor: (Initialise all context)
    TeleTall(const size_t &tltl_Window_Width, const size_t &tltl_Window_Height);
    //Destructor: (Free all resource)
    ~TeleTall();

    //Running Program: while loop here
    void Run(
        size_t tltl_Frame_Rate,
        Telepad &_ref_telepad,
        Tallwindow &_ref_tallwindow,
        Telecontroller &controller);

private:
    void ErrorReporter(const char *errorMessage);

    SDL_Window *hwnd_main;
    SDL_Renderer *hRenderer;
};

#endif