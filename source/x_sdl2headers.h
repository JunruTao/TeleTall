#ifndef X_SDL2HEADERS_H
#define X_SDL2HEADERS_H

/////////////////////////////TELETALL 2020///////////////////////////
////IF Building on Linux, please change this to _WINUSER_ to 0///////
/////////////////////////////////////////////////////////////////////
#define _WINUSER_ 1

#if _WINUSER_ == 1
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#else
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#endif

#endif