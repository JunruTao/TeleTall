#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include "class.h"

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 800

//[handle 1]The window we'll be rendering to
SDL_Window *gWindow = NULL;
//[handle 2]The surface contained by the window
SDL_Surface *gScreenSurface = NULL;
//[handle 3]The image we will load and show on the screen
SDL_Surface *gHelloWorld = NULL;

bool ContextInit()
{
    bool success = true;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER ) < 0) //********* if using delay"SDL_INIT_TIMER" this must be included here
    {
        std::cout << "SLD could not initialise! SDL_error:\n"
                  << SDL_GetError() << std::endl;
        success = false;
    }
    else
    {
        //Creating the window:
        gWindow = SDL_CreateWindow(
            "My first SDL window",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN);
        if (gWindow == NULL)
        {
            std::cout << "window could not be created! SDL_error:\n"
                      << SDL_GetError() << std::endl;
            success = false;
        }
        else
        {
            //Get Window surface:
            gScreenSurface = SDL_GetWindowSurface(gWindow);
        }
    }
    return success;
}

bool LoadMedia()
{
    bool success = true;

    gHelloWorld = SDL_LoadBMP("Hi.bmp"); //note:: must place bitmaps under same directory of executable
    if (gHelloWorld == NULL)
    {
        std::cout << "Image could not be loaded, SDL_error:\n"
                  << SDL_GetError() << std::endl;
                  success = false;
    }

    return success;
}

void Close()
{
    //Deallocate image resource data
    SDL_FreeSurface( gHelloWorld);
    gHelloWorld = NULL;

    //Destroy the window;
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    //Quit SDL subsystem
    SDL_Quit();
}

int main( int argc, char* args[] )
{
    if (ContextInit()) //************
    {
        if (LoadMedia())
        {
        //Apply the image:
        
        Hello a;
        int x = a.x;
            SDL_BlitSurface(gHelloWorld, NULL, gScreenSurface, NULL);
            SDL_UpdateWindowSurface(gWindow);

            //wait 2 sec: - in milliseconds
            SDL_Delay(1000);
            Close();
        }else
        {
            std::cout << "SDL Failed to load media!\n";
        }
    }
    else
    {
        std::cout << "SDL Failed to open window \n";
    }
    
    //free resources, close;
    
    return 0;
}