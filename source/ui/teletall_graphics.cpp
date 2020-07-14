#include "teletall_graphics.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

TTF_Font *ScreenText::gFont = NULL;
int ScreenText::counter = 0;


void ScreenText::LoadMedia(int textsize)
{
    gFont = TTF_OpenFont("tltl_fonts/arial.ttf", textsize);
    if (gFont == NULL)
    {
        //SDL_ShowSimpleMessageBox(0, "TTF Error", "Font open failed", NULL);
    }
}


ScreenText::ScreenText(int textsize)
{

    LoadMedia(textsize); //will only be loaded once
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

ScreenText::~ScreenText()
{
    Free();
    TTF_CloseFont(gFont);
    gFont = NULL;
}


void ScreenText::Free()
{
    if (mTexture != NULL)
    {
        SDL_DestroyTexture(mTexture);
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}


void ScreenText::loadFromRenderedText(
    std::string textureText,
    SDL_Color textColor,
    SDL_Color backgroundColor,
    SDL_Renderer *renderer,
    int mode
    )
{
    //Get rid of preexisting texture:
    Free();

    SDL_Surface *textSurface = NULL;
    if (mode == 1)
    {
        textSurface = TTF_RenderText_Shaded(gFont, textureText.c_str(), textColor, backgroundColor);
    }else if(mode == 0)
    {
        textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
    }else
    {
        textSurface = TTF_RenderText_Blended(gFont, textureText.c_str(), textColor);
    }


    if (textSurface != NULL)
    {
        mTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (mTexture == NULL)
        {
            SDL_ShowSimpleMessageBox(0, "TTF Error", "unable to create texture", NULL);
        }
        else
        {
            //Get image dimensions
            mWidth = textSurface->w;
            mHeight = textSurface->h;
        }
        //Get rid of old surface
        SDL_FreeSurface(textSurface);
    }
    else
    {
         SDL_ShowSimpleMessageBox(0, "TTF Error", "unable to render text surface", NULL);
    }
}


void ScreenText::Draw(SDL_Renderer* renderer, int x, int y)
{
    SDL_Rect renderrec = {x,y,mWidth,mHeight};
    SDL_RenderCopy(renderer,mTexture,NULL,&renderrec);
}