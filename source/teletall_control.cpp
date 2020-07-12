#include "teletall_control.h"
#include "SDL2/SDL.h"

size_t Telecontroller::lastMouseLocation_X = 0;
size_t Telecontroller::lastMouseLocation_Y = 0;
size_t Telecontroller::nowMouseLocation_X = 0;
size_t Telecontroller::nowMouseLocation_Y = 0;

bool Telecontroller::RMB_hold = false;
bool Telecontroller::LMB_hold = false;
bool Telecontroller::MMB_hold = false;

Telecontroller::Telecontroller()
{
    _tltlvertprop = 0;
}

Telecontroller::~Telecontroller()
{
}

void Telecontroller::ProcessInput(bool &running)
{

    SDL_Event event;
    SDL_PollEvent(&event);

    //-------------------------------------[Quit Only]
    if (event.type == SDL_QUIT)
    {
        running = false;
    }

    //--------------------------------------[Mouse press]
    else if (event.type == SDL_MOUSEBUTTONDOWN)
    {
        switch (event.button.button)
        {
        case SDL_BUTTON_LEFT:
            LMB_hold = true;
            break;
        case SDL_BUTTON_RIGHT:
            RMB_hold = true;

            break;
        default:
            MMB_hold = true;
            RMB_hold = false;
            LMB_hold = false;
            break;
        }
    }
    //--------------------------------------[Mouse Moving]
    //else if (event.type == SDL_MOUSEMOTION)
    //{   }
    //--------------------------------------[Mouse Release]
    else if (event.type == SDL_MOUSEBUTTONUP)
    {
        RMB_hold = false;
        LMB_hold = false;
        MMB_hold = false;
    }
    //--------------------------------------[Nothing]
    nowMouseLocation_X = event.motion.x;
} //end of ProcessInput scope