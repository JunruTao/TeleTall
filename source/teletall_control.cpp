#include "teletall_control.h"



Telecontroller::Telecontroller()
{
    Shared_Nevigation_Lock = MouseLockID::FREE;
    _mouseLocation.New(0, 0);
    console = new Console;
    cmd = cmd_KEY::cmd_EMPTY;
    key_ctrl = false;
    key_alt = false;
    key_shift = false;
    MouseM_hold = false;
    MouseL_hold = false;
    MouseR_hold = false;
}

Telecontroller::~Telecontroller() 
{
   delete console;
}

void Telecontroller::SendCommandEx(const cmd_KEY& _cmd, std::string _msg)
{
    cmd = _cmd;
    _Msg = _msg;
    console->Update(cmd, _Msg);
}

/*MAIN PROCESSING UNIT - PROCESS ALL INPUTS*/
void Telecontroller::ProcessInput(bool &running)
{

    SDL_Event event;
    SDL_PollEvent(&event);

        if (event.type == SDL_WINDOWEVENT)
        {
            switch (event.window.event)
            {
            case SDL_WINDOWEVENT_RESIZED:         
                break;
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                cmd = cmd_KEY::cmd_WINDOWSIZE;
                break;

            default:
                cmd = cmd_KEY::cmd_EMPTY;
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
                MouseL_hold = true;
                cmd = cmd_KEY::cmd_LMB;
                break;
            case SDL_BUTTON_RIGHT:
                MouseR_hold = true;
                cmd = cmd_KEY::cmd_RMB;
                break;
            default:
                MouseM_hold = true;
                cmd = cmd_KEY::cmd_MMB;
                break;
            }
        }
        //--------------------------------------[Mouse Moving]
        //else if (event.type == SDL_MOUSEMOTION)
        //{   }
        //--------------------------------------[Mouse Release]
        else if (event.type == SDL_MOUSEBUTTONUP)
        {
            MouseM_hold = false;
            MouseL_hold = false;
            MouseR_hold = false;
            cmd = cmd_KEY::cmd_EMPTY;
        }
        else if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_SEMICOLON:
                if(key_ctrl)
                {
                    cmd = cmd_KEY::cmd_CONSOLE_SHOW;
                }
                break;
            case SDLK_h:
                cmd = cmd_KEY::cmd_HOME;
                break;
            case SDLK_f:
                cmd = cmd_KEY::cmd_FRAME;
                break;
            case SDLK_LCTRL:
                key_ctrl = true;
                break;
           case SDLK_RCTRL:
                key_ctrl = true;
                break;
            case SDLK_LSHIFT:
                key_shift = true;
                break;
            case SDLK_RSHIFT:
                key_shift = true;
                break;
            case SDLK_LALT:
                key_alt = true;
                break;
            case SDLK_RALT:
                key_alt = true;
                break;
            default:
                break;
            }
        }
        else if (event.type == SDL_KEYUP)
        {
            cmd = cmd_KEY::cmd_EMPTY;
            key_ctrl = false;
            key_alt = false;
            key_shift = false;
        }else
        {
            cmd = cmd_KEY::cmd_EMPTY;
            _Msg.clear();
        }
        //--------------------------------------[Nothing]
        int _x, _y;
        SDL_GetMouseState(&_x, &_y);
        _mouseLocation.New(_x, _y);
        console->Update(cmd, _Msg);
        
} //end of ProcessInput scope


//[old-comm] for Tallwindows to get current split location
int Telecontroller::GetSplitLocation()const
{
    return _split_location;
}
//[old-comm] Uplink Telepad's slider bar's location 
void Telecontroller::UpdateSplitLocation(int new_split)
{
    _split_location = new_split;
}
//Get Current mouse location
Point2D<int>* Telecontroller::GetMousePoint()
{
    return &_mouseLocation;
}

//Uplink TelePad's section rectangles to control
void Telecontroller::LinkPadRect(SDL_Rect *in_PadRect, SDL_Rect *in_MSlidRec)
{
    r_PadRect.x = in_PadRect->x;
    r_PadRect.y = in_PadRect->y;
    r_PadRect.w = in_PadRect->w;
    r_PadRect.h = in_PadRect->h;
    r_MSliderRect.x = in_MSlidRec->x;
    r_MSliderRect.y = in_MSlidRec->y;
    r_MSliderRect.w = in_MSlidRec->w;
    r_MSliderRect.h = in_MSlidRec->h;
}


//Uplink Tall-window section rectangle to control
void Telecontroller::LinkTallRec(SDL_Rect *in_TallRect)
{
    r_TallRect.x = in_TallRect->x;
    r_TallRect.y = in_TallRect->y;
    r_TallRect.w = in_TallRect->w;
    r_TallRect.h = in_TallRect->h;
}



//Drawing the selection rectangles
void Telecontroller::DrawSelectionRect(SDL_Renderer* renderer)
{
    SDL_Rect consolerect;
    if (console->IsShown())
    {
        int ch = console->GetHeight();
        r_PadRect.h -= ch;
        r_TallRect.h -= ch;
        r_MSliderRect.h -= ch;

        int x;
        int y;
        SDL_GetWindowSize(*_hhwnd, &x, &y);
        if (_mouseLocation.InBound(0, y - ch, x, y))
        {
            current_panel = PanelID::ON_CONSOLE;
            consolerect = {0, y - ch, x, ch};
        }
    }
    console->Draw(_hhwnd, renderer);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    switch (current_panel)
    {
    case PanelID::ON_PAD:
        SDL_SetRenderDrawColor(renderer, 90, 143, 222, 255);
        SDL_RenderDrawRect(renderer, &r_PadRect);
        break;
    case PanelID::ON_BAR:
        SDL_SetRenderDrawColor(renderer, 90, 143, 222, 150);
        SDL_RenderDrawRect(renderer, &r_MSliderRect);
        break;
    case PanelID::ON_TALL:
        SDL_SetRenderDrawColor(renderer, 0, 153, 153, 210);
        SDL_RenderDrawRect(renderer, &r_TallRect);
        break;
    case PanelID::ON_CONSOLE:
        SDL_SetRenderDrawColor(renderer, 90, 143, 222, 255);
        SDL_RenderDrawRect(renderer, &consolerect);
    default:
        break;
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}