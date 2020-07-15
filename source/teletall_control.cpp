#include "teletall_control.h"

bool Telecontroller::RMB_hold = false;
bool Telecontroller::LMB_hold = false;
bool Telecontroller::MMB_hold = false;

Telecontroller::Telecontroller()
{
    key_FRAME = false;
    key_HOME = false;
    eWinUpdate = false;
    _mouseLocation.New(0,0);
}

Telecontroller::~Telecontroller() {}


void Telecontroller::ProcessInput(bool &running)
{

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {

        if (event.type == SDL_WINDOWEVENT)
        {
            switch (event.window.event)
            {
            case SDL_WINDOWEVENT_RESIZED:
                eWinUpdate = true;
                break;
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                eWinUpdate = true;
                break;

            default:
                eWinUpdate = false;
                break;
            }
        }

        //-------------------------------------[Quit Only]
        else if (event.type == SDL_QUIT)
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
        int _x, _y;
        SDL_GetMouseState(&_x, &_y);
        _mouseLocation.New(_x, _y);
    }

} //end of ProcessInput scope



int Telecontroller::GetSplitLocation()const
{
    return _split_location;
}

void Telecontroller::UpdateSplitLocation(int new_split)
{
    _split_location = new_split;
}


Point2D<int>* Telecontroller::GetMousePoint()
{
    return &_mouseLocation;
}