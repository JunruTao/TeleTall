#ifndef TELETALL_GRAPHICS_H
#define TELETALL_GRAPHICS_H

#include "x_sdl2headers.h"

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
    if (origin.x >= 0 && pad_width >= grid_size)
    {
        if (origin.x <= pad_width)
        {
            distance = origin.x;
            while (distance >= 0 && distance <= pad_width)
            {
                SDL_RenderDrawLine(renderer, distance, 0, distance, pad_height);
                distance -= grid_size;
            }
            distance = origin.x;
            while (distance <= pad_width)
            {
                SDL_RenderDrawLine(renderer, distance, 0, distance, pad_height);
                distance += grid_size;
            }
        }
        else
        {
            distance = origin.x;
            while (distance >= pad_width)
            {
                distance -= grid_size;
            }
            while (distance >= 0 && distance <= pad_width)
            {
                SDL_RenderDrawLine(renderer, distance, 0, distance, pad_height);
                distance -= grid_size;
            }
        }
    }
    else if (pad_width >= grid_size)
    {
        distance = origin.x;
        while (distance <= 0)
        {
            distance += grid_size;
        }
        while (distance <= pad_width)
        {
            SDL_RenderDrawLine(renderer, distance, 0, distance, pad_height);
            distance += grid_size;
        }
    }
    //---------------------------------
    //Draw Horizontal
    distance = 0;
    if (origin.y >= 0 && pad_height >= grid_size)
    {
        if (origin.y <= pad_height)
        {
            distance = origin.y;
            while (distance >= 0 && distance <= pad_height)
            {
                SDL_RenderDrawLine(renderer, 0, distance, pad_width, distance);
                distance -= grid_size;
            }
            distance = origin.y;
            while (distance <= pad_height)
            {
                SDL_RenderDrawLine(renderer, 0, distance, pad_width, distance);
                distance += grid_size;
            }
        }
        else
        {
            distance = origin.y;
            while (distance >= pad_height)
            {
                distance -= grid_size;
            }
            while (distance > 0 && distance <= pad_height)
            {
                SDL_RenderDrawLine(renderer, 0, distance, pad_width, distance);
                distance -= grid_size;
            }
        }
    }
    else if(pad_height >= grid_size)
    {
        distance = origin.y;
        while (distance <= 0)
        {
            distance += grid_size;
        }
        while (distance <= pad_height)
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

static void DrawCross_D
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

    SDL_RenderDrawLine(renderer, origin.x + cross_size, origin.y+1, origin.x - cross_size, origin.y+1);
    SDL_RenderDrawLine(renderer, origin.x+1, origin.y + cross_size, origin.x+1, origin.y - cross_size);
    SDL_RenderDrawLine(renderer, origin.x + cross_size, origin.y-1, origin.x - cross_size, origin.y-1);
    SDL_RenderDrawLine(renderer, origin.x-1, origin.y + cross_size, origin.x-1, origin.y - cross_size);
}

static void DrawCross_B
(
    SDL_Renderer *renderer,
    Point2D<int> &origin,
    const int& cross_size,
    const int& r_255,
    const int& g_255,
    const int& b_255
)
{
    SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
    SDL_RenderDrawLine(renderer, origin.x + cross_size, origin.y + 1, origin.x - cross_size, origin.y + 1);
    SDL_RenderDrawLine(renderer, origin.x + 1, origin.y + cross_size, origin.x + 1, origin.y - cross_size);
    SDL_RenderDrawLine(renderer, origin.x + cross_size, origin.y - 1, origin.x - cross_size, origin.y - 1);
    SDL_RenderDrawLine(renderer, origin.x - 1, origin.y + cross_size, origin.x - 1, origin.y - cross_size);

    SDL_SetRenderDrawColor(renderer, r_255, g_255, b_255, 255);
    SDL_RenderDrawLine(renderer, origin.x + cross_size, origin.y, origin.x - cross_size, origin.y);
    SDL_RenderDrawLine(renderer, origin.x, origin.y + cross_size, origin.x, origin.y - cross_size);
}

static void DrawLineExtendToBound(
    SDL_Renderer *renderer,
    const Point2D<int>* origin,
    const SDL_Rect *rectangle,
    int &&axis)
{
    if (axis == 1)
    {
        SDL_RenderDrawLine(renderer, origin->x, rectangle->y, origin->x, rectangle->h + rectangle->y);
    }else if(axis == 0)
    {
        SDL_RenderDrawLine(renderer, rectangle->x, origin->y, rectangle->w + rectangle->x, origin->y);
    }
}



//inplemented in screen space
class ScreenText
{
public:
    //constructor/deconstructor
    ScreenText();
    ~ScreenText();
    
    void loadFromRenderedText(
        std::string textureText, 
        SDL_Renderer* renderer,
        SDL_Color textColor,
        SDL_Color backgroundColor = {(Uint8)255,(Uint8)255,(Uint8)255},
        int mode = 0 //0=solid, 1=shaded, 3=blend
        );

    void Draw(SDL_Renderer* renderer,int x, int y, double scale = 1);
    static void InitLoadMedia(int textsize);
    static int GetUniversalTextHeight();
    int GetHeight(){return uniHeight;}
    int GetWidth(){return mWidth;}
    

private:
    void Free();
    
    //The actual hardware texture
    SDL_Texture *mTexture;
    //Image dimensions
    int mWidth;
    int mHeight;
    static TTF_Font* gFont;
    static int counter;
    static int uniHeight;
};



#endif