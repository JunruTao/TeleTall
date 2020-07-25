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
    xkey_hold = false;
    _closeAllMenus = false;
    EditMode = false;
    clickstamp = 0;
    _selected_nodes = 0;
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

    //---------------------------------
    switch (cmd)//Get commands from menu
    {
    case cmd_KEY::cmd_EditMode_M:
        EditMode = !EditMode;
        if (EditMode)
        {
            _Msg = " Entering Edit Mode";
        }
        else
        {
            _Msg = " Exit Edit Mode";
        }
        break;
    case cmd_KEY::cmd_WIN_ABOUT:

        about_text = "                       TeleTall Software Copyright. 2020\n\n";
        about_text += "Teletall is a simple node-based procedural 2D modelling application\n";
        about_text += "This project was made for the Capstone project in the Udacity C++\n";
        about_text += "Nanodegree Program. The Capstone Project is a chance to integrate\n";
        about_text += "what I've learned throughout this program. \n\n";
        about_text += "Author: Junru Tao";

        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "   About",  about_text.c_str(), *_hhwnd);
        break;
    case cmd_KEY::cmd_GRID_SNAP_M:
        xkey_hold = !xkey_hold;
        if(xkey_hold)
        {
            SendCommandEx(cmd_KEY::cmd_Message, " Grid Snap {ON}");
        }else
        {
            SendCommandEx(cmd_KEY::cmd_Message, " Grid Snap {OFF}");
        }
        break;
    default:
        break;
    }
    //---------------------------

    int _x, _y;
    SDL_GetMouseState(&_x, &_y);
    _mouseLocation.New(_x, _y);
    SDL_Event event;

    current_panel = PanelID::NONE;
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
    else if (event.type == SDL_QUIT || cmd == cmd_KEY::cmd_QUIT)
    {
        _closeAllMenus = false;
        running = false;
    }

    //--------------------------------------[Mouse press]
    else if (event.type == SDL_MOUSEBUTTONDOWN)
    {
        clickstamp = event.button.timestamp;

        switch (event.button.button)
        {
        case SDL_BUTTON_LEFT:
            if (EditMode && event.button.clicks == 2)
            {
                cmd = cmd_KEY::cmd_CREATE_POINT;
            }else if(_selected_nodes != 0 && event.button.clicks == 2)
            {
                cmd = cmd_KEY::cmd_DisplayFlag;
            }
            else
            {
                cmd = cmd_KEY::cmd_LMB;
                if (!MouseL_hold)
                {
                    MouseL_hold = true;
                }
            }
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
    else if (event.type == SDL_MOUSEWHEEL)
    {
        if (event.wheel.y > 0) // scroll up
        {
            cmd = cmd_KEY::cmd_ZOOM_IN;
        }
        else if (event.wheel.y < 0) // scroll down
        {
            cmd = cmd_KEY::cmd_ZOOM_OUT;
        }
    }
    //--------------------------------------[Mouse Moving]
    //else if (event.type == SDL_MOUSEMOTION)
    //{   }
    //--------------------------------------[Mouse Release]
    else if (event.type == SDL_MOUSEBUTTONUP)
    {
        clicktime = event.button.timestamp - clickstamp;
        _closeAllMenus = false;
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
            if (key_ctrl)
            {
                cmd = cmd_KEY::cmd_CONSOLE_SHOW;
            }
            break;
        case SDLK_p:
            cmd = cmd_KEY::cmd_CREATE_NODE_Point;
            break;
        case SDLK_d:
            cmd = cmd_KEY::cmd_DisplayFlag;
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
        case SDLK_x:
            xkey_hold = !xkey_hold;
            if (xkey_hold)
            {
                cmd = cmd_KEY::cmd_Message; _Msg =  " Grid Snap {ON} x";
            }
            else
            {
                cmd = cmd_KEY::cmd_Message; _Msg =  " Grid Snap {OFF} x";
            }
            break;
        case SDLK_DELETE:
            cmd = cmd_KEY::cmd_Delete;
            break;
        case SDLK_RETURN:
            if (!EditMode)
            {
                EditMode = true;
                cmd = cmd_KEY::cmd_EditMode;
                _Msg = " Entering Edit Mode";
            }
            break;
        case SDLK_ESCAPE:
            if (EditMode)
            {
                EditMode = false;
                cmd = cmd_KEY::cmd_EditMode;
                _Msg = " Exit Edit Mode";
            }
            else
            {
                cmd = cmd_KEY::cmd_CLEAR_Sel;
            }
            break;
        case SDLK_SPACE:
            cmd = cmd_KEY::cmd_EditMode;
            EditMode = !EditMode;
            if (EditMode)
            {
                _Msg = " Entering Edit Mode";
            }
            else
            {
                _Msg = " Exit Edit Mode";
            }
            break;
        case SDLK_z:
            if (key_ctrl)
            {
                cmd = cmd_KEY::cmd_UNDO;
            }
            break;
        case SDLK_1:
            cmd = cmd_KEY::cmd_CNLINE_1_LINE;
            break;
        case SDLK_2:
            cmd = cmd_KEY::cmd_CNLINE_2_ZIG;
            break;
        case SDLK_3:
            cmd = cmd_KEY::cmd_CNLINE_3_CURVE;
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
        _closeAllMenus = false;
    }
    else
    {
        cmd = cmd_KEY::cmd_EMPTY;
        _Msg.clear();
        _closeAllMenus = false;
    }
    //--------------------------------------[Nothing]
    if (EditMode)
    {
        current_panel = PanelID::ON_TALL;
        Shared_Nevigation_Lock = MouseLockID::TALL_LOCKED;
    }
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

void Telecontroller::LinkMenuRec(SDL_Rect* in_Menu)
{
    r_MenuRect = *in_Menu;
}



//Drawing the selection rectangles
void Telecontroller::DrawSelectionRect(SDL_Renderer* renderer)
{

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
            r_ConsoleRect = {0, y - ch, x, ch};
        }
    }
    r_PadRect.h -= r_MenuRect.h;
    r_TallRect.h -= r_MenuRect.h;
    r_MSliderRect.h -= r_MenuRect.h;
    r_PadRect.y += r_MenuRect.h;
    r_TallRect.y += r_MenuRect.h;
    r_MSliderRect.y += r_MenuRect.h;

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
        if (EditMode)
        {
            SDL_SetRenderDrawColor(renderer, 255, 30, 140, 210);
        }
        else
        {
            SDL_SetRenderDrawColor(renderer, 0, 153, 153, 210);
        }
        SDL_RenderDrawRect(renderer, &r_TallRect);
        break;
    case PanelID::ON_CONSOLE:
        SDL_SetRenderDrawColor(renderer, 90, 143, 222, 255);
        SDL_RenderDrawRect(renderer, &r_ConsoleRect);
    case PanelID::ON_MENU:
        break;
    default:
        break;
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}