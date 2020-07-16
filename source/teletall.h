#ifndef TELETALL_H
#define TELETALL_H

#include "x_sdl2headers.h"

#include "telepad.h"
#include "tallwindow.h"
#include "teletall_control.h"
#include "ui/teletall_graphics.h"
#include "teletall_menu.h"

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
    std::string win_title;
    SDL_Window *hwnd_main;
    SDL_Renderer *hRenderer;
};

#endif