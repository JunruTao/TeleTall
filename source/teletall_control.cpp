#include "teletall_control.h"

int Telecontroller::lastMouseLocation_X = 0;
int Telecontroller::lastMouseLocation_Y = 0;
int Telecontroller::nowMouseLocation_X = 0;
int Telecontroller::nowMouseLocation_Y = 0;

bool Telecontroller::RMB_hold = false;
bool Telecontroller::LMB_hold = false;
bool Telecontroller::MMB_hold = false;

Telecontroller::Telecontroller()
{
    key_FRAME = false;
    key_HOME = false;
}

Telecontroller::~Telecontroller() {}


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
        SDL_GetMouseState(&nowMouseLocation_X, &nowMouseLocation_Y);
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
    else if (event.type == SDL_KEYDOWN)
    {
        switch (event.key.keysym.sym)
        {
        case SDLK_h:
            key_HOME = true;
            break;
        case SDLK_f:
            key_FRAME = true;
            break;

        default:
            break;
        }
    }
    else if (event.type == SDL_KEYUP)
    {
        key_HOME = false;
        key_FRAME = false;
    }
    //--------------------------------------[Nothing]
} //end of ProcessInput scope



int Telecontroller::GetSplitLocation()const
{
    return _split_location;
}

void Telecontroller::UpdateSplitLocation(int new_split)
{
    _split_location = new_split;
}