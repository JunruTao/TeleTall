#include "telepad.h"

#define SLIDEBAR_WIDTH 12
#define HALF_SLIDEBAR 6

Telepad::Telepad(
    const size_t &master_width,
    const size_t &master_height)

{
    win_width = master_width;
    win_height = master_height;

    pad_width = master_width / 2;
    pad_height = master_height;

    padViewport.x = 0;
    padViewport.y = 0;
    padViewport.w = pad_width;
    padViewport.h = pad_height;

    slidebar.x = pad_width - HALF_SLIDEBAR;
    slidebar.y = 0;
    slidebar.w = SLIDEBAR_WIDTH;
    slidebar.h = pad_height;

    slidebarSelected = false;
}

Telepad::~Telepad()
{
}

void Telepad::Update(const Telecontroller &controller)
{
    //current mouse location
    int x, y;
    SDL_GetMouseState(&x, &y);

    SDL_Cursor *cursor;
    bool onBar = (x > slidebar.x - 5) && (x < slidebar.x + slidebar.w + 5);
    bool onPad = (x > 0) && (x < pad_width-HALF_SLIDEBAR-5);
    if (onBar)
    {
        //changing cursor
        cursor = init_system_cursor(doublearrow);
        SDL_SetCursor(cursor);
    }
    else if(onPad)
    {
        //changing cursor
        cursor = init_system_cursor(cross);
        SDL_SetCursor(cursor);
    }
    else
    {
        cursor = SDL_GetDefaultCursor();
        SDL_SetCursor(cursor);
    }

    if (controller.LMB_hold)
    {
        if (onBar || slidebarSelected)
        {
            cursor = init_system_cursor(doublearrow);
            SDL_SetCursor(cursor);

            slidebarSelected = true;
            pad_width = x;
            JUTA_Math::Clamp<size_t>(pad_width, 20, win_width - 20);
            padViewport.w = pad_width;
            slidebar.x = pad_width - HALF_SLIDEBAR;
        }
    }
    else
    {

        slidebarSelected = false;
    }
}

void Telepad::Render(SDL_Renderer *renderer)
{
    //background region here
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_RenderFillRect(renderer, &padViewport);
    //clipping here
    SDL_RenderSetClipRect(renderer, &padViewport);
    //render nodes and geo's here:

//Some Sample gird---------------------------
    SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
        for (int i = 0; i < 20; i++)
        {
            int y = 30 *i+1;
            SDL_RenderDrawLine(renderer,0,y,900,y );
        }
        for (int i = 0; i < 30; i++)
        {
            int x = 30 *i+1;
            SDL_RenderDrawLine(renderer,x,0,x,600 );
        }
        //Some Sample gird---------------------------

    SDL_RenderSetClipRect(renderer, NULL);
    SDL_SetRenderDrawColor(renderer, 130, 130, 130, 255);
    SDL_RenderFillRect(renderer, &slidebar);

    SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255);
    SDL_RenderDrawLine(renderer, pad_width - HALF_SLIDEBAR, 0, pad_width - HALF_SLIDEBAR, pad_height);
    SDL_RenderDrawLine(renderer, pad_width + HALF_SLIDEBAR, 0, pad_width + HALF_SLIDEBAR, pad_height);
    SDL_SetRenderDrawColor(renderer, 90, 90, 90, 255);
    SDL_RenderDrawLine(renderer, pad_width - HALF_SLIDEBAR + 1, 0, pad_width - HALF_SLIDEBAR + 1, pad_height);
    SDL_RenderDrawLine(renderer, pad_width + HALF_SLIDEBAR - 1, 0, pad_width + HALF_SLIDEBAR - 1, pad_height);
    SDL_SetRenderDrawColor(renderer, 110, 110, 110, 255);
    SDL_RenderDrawLine(renderer, pad_width - HALF_SLIDEBAR + 1, 0, pad_width - HALF_SLIDEBAR + 1, pad_height);
    SDL_RenderDrawLine(renderer, pad_width + HALF_SLIDEBAR - 1, 0, pad_width + HALF_SLIDEBAR - 1, pad_height);
    SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255);
    SDL_RenderDrawLine(renderer, pad_width - HALF_SLIDEBAR + 2, 0, pad_width - HALF_SLIDEBAR + 2, pad_height);
    SDL_RenderDrawLine(renderer, pad_width + HALF_SLIDEBAR - 2, 0, pad_width + HALF_SLIDEBAR - 2, pad_height);
}