#include <stdio.h>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "class.h"
/*
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
}*/

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image
SDL_Surface* loadSurface( std::string path );

//The window we'll be rendering to
SDL_Window* gWindow = NULL;
	
//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//Current displayed PNG image
SDL_Surface* gPNGSurface = NULL;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Initialize PNG loading
			int imgFlags = IMG_INIT_PNG;
			if( !( IMG_Init( imgFlags ) & imgFlags ) )
			{
				printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
				success = false;
			}
			else
			{
				//Get window surface
				gScreenSurface = SDL_GetWindowSurface( gWindow );
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load PNG surface
	gPNGSurface = loadSurface( "a.png" );
	if( gPNGSurface == NULL )
	{
		printf( "Failed to load PNG image!\n" );
		success = false;
	}

	return success;
}

void close()
{
	//Free loaded image
	SDL_FreeSurface( gPNGSurface );
	gPNGSurface = NULL;

	//Destroy window
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

SDL_Surface* loadSurface( std::string path )
{
	//The final optimized image
	SDL_Surface* optimizedSurface = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Convert surface to screen format
		optimizedSurface = SDL_ConvertSurface( loadedSurface, gScreenSurface->format, 0 );
		if( optimizedSurface == NULL )
		{
			printf( "Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	return optimizedSurface;
}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
				}

				//Apply the PNG image
				SDL_BlitSurface( gPNGSurface, NULL, gScreenSurface, NULL );
			
				//Update the surface
				SDL_UpdateWindowSurface( gWindow );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}