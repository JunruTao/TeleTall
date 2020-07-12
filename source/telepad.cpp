#include "telepad.h"
#include "JUTA/JUTA_math.h"
#include "JUTA/JUTA_geometry_core.h"

#define SLIDEBAR_WIDTH 12
#define HALF_SLIDEBAR 6
#define SLIDEBAR_SEL_BLEED 5
#define TELEPAD_GRID_SIZE 60
//--------------------------------------------------------------------------------------
Telepad::Telepad(
    const size_t &master_width,
    const size_t &master_height)

{
    //Setting up dimension local varible
    win_width = master_width;
    win_height = master_height;
    pad_width = master_width / 2;
    pad_height = master_height;

    //viewport size init
    padViewport.x = 0;
    padViewport.y = 0;
    padViewport.w = pad_width;
    padViewport.h = pad_height;

    //Slide bar size init
    slidebar.x = pad_width - HALF_SLIDEBAR;
    slidebar.y = 0;
    slidebar.w = SLIDEBAR_WIDTH;
    slidebar.h = pad_height;

    //Slide bar seletion init
    slidebarSelected = false;
    gridSelected = false;

    //world grid Starting Point;
    origin.x = pad_width / 2;
    origin.y = pad_height / 2;
    grid_size = TELEPAD_GRID_SIZE;
}
/*
#[[[[[[[[[[[]]]]]]]]]]]
#[[[[[[[[[[[]]]]]]]]]]]
*/
//--------------------------------------------------------------------------------------
Telepad::~Telepad() {}
/*
#[[[[[[[[[[[]]]]]]]]]]]
#[[[[[[[[[[[]]]]]]]]]]]
*/
Point2D<int> *Telepad::GetOrigin()
{
    return &origin;
}
/*
#[[[[[[[[[[[]]]]]]]]]]]
#[[[[[[[[[[[]]]]]]]]]]]
*/
//--------------------------------------------------------------------------------------
void Telepad::Update(Telecontroller &controller)
{
    //current mouse location
    int x, y;
    SDL_GetMouseState(&x, &y);
    Resize(controller, x, y);
    MoveGrid(controller, x, y);

    controller.UpdateSplitLocation(pad_width + HALF_SLIDEBAR + SLIDEBAR_SEL_BLEED);
}
/*
#
#
#
#
*/
//--------------------------------------------------------------------------------------
void Telepad::Render(SDL_Renderer *renderer)
{
    //background region here
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_RenderFillRect(renderer, &padViewport);
    //clipping here
    SDL_RenderSetClipRect(renderer, &padViewport);
    //Draw gird---------------------------
    DrawGrid(renderer,origin,grid_size,pad_width,pad_height,70);
    //render nodes and geo's here:
    if(origin.InBound(0,0,pad_width,pad_height))
    {
        DrawCross(renderer,origin,5,110,110,110);
    }
    //..

    //Draw slider---------------------------
    DrawSlideBar(renderer);
}
/*
#
#
#
#
#
#
*/
//------------------------------------------------------------------------------------
void Telepad::DrawSlideBar(SDL_Renderer *renderer)
{
    SDL_RenderSetClipRect(renderer, NULL);
    SDL_SetRenderDrawColor(renderer, 130, 130, 130, 255);
    SDL_RenderFillRect(renderer, &slidebar);

    SDL_SetRenderDrawColor(renderer, 90, 90, 90, 255);
    SDL_RenderDrawLine(renderer, pad_width - HALF_SLIDEBAR, 0, pad_width - HALF_SLIDEBAR, pad_height);
    SDL_RenderDrawLine(renderer, pad_width + HALF_SLIDEBAR, 0, pad_width + HALF_SLIDEBAR, pad_height);

    SDL_RenderDrawLine(renderer, pad_width + 2, pad_height / 2 + 5, pad_width + 2, pad_height / 2 - 5);
    SDL_RenderDrawLine(renderer, pad_width, pad_height / 2 + 5, pad_width, pad_height / 2 - 5);
    SDL_RenderDrawLine(renderer, pad_width - 2, pad_height / 2 + 5, pad_width - 2, pad_height / 2 - 5);

    SDL_SetRenderDrawColor(renderer, 90, 90, 90, 255);
    SDL_RenderDrawLine(renderer, pad_width - HALF_SLIDEBAR + 1, 0, pad_width - HALF_SLIDEBAR + 1, pad_height);
    SDL_RenderDrawLine(renderer, pad_width + HALF_SLIDEBAR - 1, 0, pad_width + HALF_SLIDEBAR - 1, pad_height);
    SDL_SetRenderDrawColor(renderer, 110, 110, 110, 255);
    SDL_RenderDrawLine(renderer, pad_width - HALF_SLIDEBAR + 2, 0, pad_width - HALF_SLIDEBAR + 2, pad_height);
    SDL_RenderDrawLine(renderer, pad_width + HALF_SLIDEBAR - 2, 0, pad_width + HALF_SLIDEBAR - 2, pad_height);

    SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255);
    SDL_RenderDrawLine(renderer, pad_width - HALF_SLIDEBAR + 3, 0, pad_width - HALF_SLIDEBAR + 3, pad_height);
    SDL_RenderDrawLine(renderer, pad_width + HALF_SLIDEBAR - 3, 0, pad_width + HALF_SLIDEBAR - 3, pad_height);
}
/*
#
#
#

#
#
*/
//--------------------------------------------------------------------------------------
void Telepad::Resize(Telecontroller &controller, const int &x, const int &y)
{
    SDL_Cursor *cursor = NULL;
    bool onBar = (x > slidebar.x - SLIDEBAR_SEL_BLEED) && (x < slidebar.x + slidebar.w + SLIDEBAR_SEL_BLEED);
    bool onTall = (x > slidebar.x + slidebar.w + SLIDEBAR_SEL_BLEED) && (x < win_width);

 

    if (onBar) //Mouse passing the slide bar
    {
        cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEWE);
        SDL_SetCursor(cursor);
        controller.current_panel = PanelID::ON_BAR;
        //changing cursor
    }

    else if (onTall) //Mouse passing rest of the area
    {
        cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_CROSSHAIR);
        SDL_SetCursor(cursor);
    }

    if (controller.LMB_hold) //Mouse clicked down
    {
        if (onBar || slidebarSelected) //Hold or Grag
        {
            controller.current_panel = PanelID::ON_BAR;
            cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);
            SDL_SetCursor(cursor);
            slidebarSelected = true;
            pad_width = x;
            JUTA_Math::Clamp<size_t>(pad_width, 20, win_width - 20);

            padViewport.w = pad_width;              //Updating viewport's location
            slidebar.x = pad_width - HALF_SLIDEBAR; //updating slidebar's location
        }
    }
    else
    {
        slidebarSelected = false; //Set seleted state to false
    }

    if(onTall) controller.current_panel = PanelID::ON_TALL;
}
/*
#
#
#
#
#
#
#
#
#
#
*/
//-------------------------------------------------------------------------------------
void Telepad::MoveGrid(Telecontroller &controller, const int &x, const int &y)
{
    SDL_Cursor *cursor = NULL;
    bool onPad = (x > 0) && (x < pad_width - HALF_SLIDEBAR);
    if (controller.RMB_hold)
    {
        if (onPad)
        {
            controller.current_panel = PanelID::ON_PAD;
            cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);
            SDL_SetCursor(cursor);
        }

        if (mouse_trail.empty())
        {
            Point2D<int> temp(x, y);
            mouse_trail.emplace_back(std::move(temp));
        }

        if (onPad || gridSelected)
        {
            controller.current_panel = PanelID::ON_PAD;
            if (mouse_trail.size() > 1)
            {
                mouse_trail.erase(mouse_trail.begin());
            }
            Point2D<int> temp(x, y);
            mouse_trail.emplace_back(std::move(temp));
            Point2D<int> motion = mouse_trail[1] - mouse_trail[0];

            if (motion.Length() > 700)
            {
                motion.x = 0;
                motion.y = 0;
            }
            origin.Translate(motion);

            //locking the dragging mode
            gridSelected = true;
        }
    }
    else if (onPad && controller.key_HOME)
    {
        origin.x = pad_width / 2;
        origin.y = pad_height / 2;
    }
    else
    {
        if (onPad)
        {
            controller.current_panel = PanelID::ON_PAD;
            cursor = SDL_GetDefaultCursor();
            SDL_SetCursor(cursor);
        }

        //after release set gimble location
        while (mouse_trail.size() > 0)
        {
            mouse_trail.pop_back();
        }
        gridSelected = false;
    }
}