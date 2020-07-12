#ifndef TELETALL_MENU_H
#define TELETALL_MENU_H

#include <SDL2/SDL.h>
#include <JUTA/JUTA_geometry_core.h>
#include <JUTA/JUTA_math.h>

static void DrawGrid(
    SDL_Renderer *renderer,
    Point2D<int> &origin,
    const int &grid_size,
    const int &pad_width,
    const int &pad_height,
    const int &grayness)
{
    //Draw -Vertical
    int distance = 0;
    SDL_SetRenderDrawColor(renderer, grayness, grayness, grayness, 255);
    if (origin.x > 0 && pad_width > grid_size)
    {
        if (origin.x <= pad_width)
        {
            distance = origin.x;
            while (distance > 0 && distance < pad_width)
            {
                SDL_RenderDrawLine(renderer, distance, 0, distance, pad_height);
                distance -= grid_size;
            }
            distance = origin.x;
            while (distance < pad_width)
            {
                SDL_RenderDrawLine(renderer, distance, 0, distance, pad_height);
                distance += grid_size;
            }
        }
        else
        {
            distance = origin.x;
            while (distance > pad_width)
            {
                distance -= grid_size;
            }
            while (distance > 0 && distance < pad_width)
            {
                SDL_RenderDrawLine(renderer, distance, 0, distance, pad_height);
                distance -= grid_size;
            }
        }
    }
    else if (pad_width > grid_size)
    {
        distance = origin.x;
        while (distance < 0)
        {
            distance += grid_size;
        }
        while (distance < pad_width)
        {
            SDL_RenderDrawLine(renderer, distance, 0, distance, pad_height);
            distance += grid_size;
        }
    }
    //---------------------------------
    //Draw Horizontal
    distance = 0;
    if (origin.y > 0)
    {
        if (origin.y <= pad_height)
        {
            distance = origin.y;
            while (distance > 0 && distance < pad_height)
            {
                SDL_RenderDrawLine(renderer, 0, distance, pad_width, distance);
                distance -= grid_size;
            }
            distance = origin.y;
            while (distance < pad_height)
            {
                SDL_RenderDrawLine(renderer, 0, distance, pad_width, distance);
                distance += grid_size;
            }
        }
        else
        {
            distance = origin.y;
            while (distance > pad_height)
            {
                distance -= grid_size;
            }
            while (distance > 0 && distance < pad_height)
            {
                SDL_RenderDrawLine(renderer, 0, distance, pad_width, distance);
                distance -= grid_size;
            }
        }
    }
    else
    {
        distance = origin.y;
        while (distance < 0)
        {
            distance += grid_size;
        }
        while (distance < pad_height)
        {
            SDL_RenderDrawLine(renderer, 0, distance, pad_width, distance);
            distance += grid_size;
        }
    }
}




static void DrawCross
(
    SDL_Renderer *renderer,
    Point2D<int> &origin,
    const int& cross_size,
    const int& r_255,
    const int& g_255,
    const int& b_255
)
{
    SDL_SetRenderDrawColor(renderer, r_255, g_255, b_255, 255);
    SDL_RenderDrawLine(renderer, origin.x + cross_size, origin.y, origin.x - cross_size, origin.y);
    SDL_RenderDrawLine(renderer, origin.x, origin.y + cross_size, origin.x, origin.y - cross_size);
}

#endif