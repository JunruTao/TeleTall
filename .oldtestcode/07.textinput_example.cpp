/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, SDL_ttf, standard IO, strings, and string streams
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <sstream>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Texture wrapper class
class LTexture
{

private:
    //The actual hardware texture
    SDL_Texture *mTexture;
    //Image dimensions
    int mWidth;
    int mHeight;

public:
    //Initializes variables
    LTexture()
    {
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }





    //Creates image from font string
    bool loadFromRenderedText(std::string textureText, SDL_Color textColor)
    {
        //Get rid of preexisting texture
        free();

        //Render text surface
        SDL_Surface *textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
        if (textSurface != NULL)
        {
            //Create texture from surface pixels
            mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
            if (mTexture == NULL)
            {
                printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
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
            printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
        }

        //Return success
        return mTexture != NULL;
    }



    //Set color modulation
    void setColor(Uint8 red, Uint8 green, Uint8 blue){SDL_SetTextureColorMod(mTexture, red, green, blue);}
    //Set blending
    void setBlendMode(SDL_BlendMode blending){SDL_SetTextureBlendMode(mTexture, blending);}
    //Set alpha modulation
    void setAlpha(Uint8 alpha){SDL_SetTextureAlphaMod(mTexture, alpha);}
    
    
    //Renders texture at given point
    void render(
        int x, int y,
        SDL_Rect *clip = NULL,
        double angle = 0.0,
        SDL_Point *center = NULL,
        SDL_RendererFlip flip = SDL_FLIP_NONE)
    {
        //Set rendering space and render to screen
        SDL_Rect renderQuad = {x, y, mWidth, mHeight};

        //Set clip rendering dimensions
        if (clip != NULL)
        {
            renderQuad.w = clip->w;
            renderQuad.h = clip->h;
        }

        //Render to screen
        SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
    }
    
    
    //Gets image dimensions
    int getWidth() { return mWidth; }
    int getHeight() { return mHeight; }
};

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();



//Globally used font
TTF_Font *gFont = NULL;
//Scene textures
LTexture gPromptTextTexture;
LTexture gInputTextTexture;


bool loadMedia()
{
    //Loading success flag
    bool success = true;

    //Open the font
    gFont = TTF_OpenFont("32_text_input_and_clipboard_handling/lazy.ttf", 28);
    if (gFont == NULL)
    {
        printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
        success = false;
    }
    else
    {
        //Render the prompt
        SDL_Color textColor = {0, 0, 0, 0xFF};
        if (!gPromptTextTexture.loadFromRenderedText("Enter Text:", textColor))
        {
            printf("Failed to render prompt text!\n");
            success = false;
        }
    }

    return success;
}


int main(int argc, char *args[])
{
    //Start up SDL and create window
    //Load media
        if (!loadMedia())
        {
            printf("Failed to load media!\n");
        }
        else
        {
            //Main loop flag
            bool quit = false;

            //Event handler
            SDL_Event e;

            //Set text color as black
            SDL_Color textColor = {0, 0, 0, 0xFF};

            //The current input text.
            std::string inputText = "Some Text";
            gInputTextTexture.loadFromRenderedText(inputText.c_str(), textColor);

            //Enable text input
            SDL_StartTextInput();

            //While application is running
            while (!quit)
            {
                //The rerender text flag
                bool renderText = false;

                //Handle events on queue
                while (SDL_PollEvent(&e) != 0)
                {
                    //User requests quit
                    if (e.type == SDL_QUIT)
                    {
                        quit = true;
                    }
                    //Special key input
                    else if (e.type == SDL_KEYDOWN)
                    {
                        //Handle backspace
                        if (e.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0)
                        {
                            //lop off character
                            inputText.pop_back();
                            renderText = true;
                        }
                        //Handle copy
                        else if (e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL)
                        {
                            SDL_SetClipboardText(inputText.c_str());
                        }
                        //Handle paste
                        else if (e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL)
                        {
                            inputText = SDL_GetClipboardText();
                            renderText = true;
                        }
                    }
                    //Special text input event
                    else if (e.type == SDL_TEXTINPUT)
                    {
                        //Not copy or pasting
                        if (!(SDL_GetModState() & KMOD_CTRL && (e.text.text[0] == 'c' || e.text.text[0] == 'C' || e.text.text[0] == 'v' || e.text.text[0] == 'V')))
                        {
                            //Append character
                            inputText += e.text.text;
                            renderText = true;
                        }
                    }
                }

                //Rerender text if needed
                if (renderText)
                {
                    //Text is not empty
                    if (inputText != "")
                    {
                        //Render new text
                        gInputTextTexture.loadFromRenderedText(inputText.c_str(), textColor);
                    }
                    //Text is empty
                    else
                    {
                        //Render space texture
                        gInputTextTexture.loadFromRenderedText(" ", textColor);
                    }
                }

                //Clear screen
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(gRenderer);

                //Render text textures
                gPromptTextTexture.render((SCREEN_WIDTH - gPromptTextTexture.getWidth()) / 2, 0);
                gInputTextTexture.render((SCREEN_WIDTH - gInputTextTexture.getWidth()) / 2, gPromptTextTexture.getHeight());

                //Update screen
                SDL_RenderPresent(gRenderer);
            }

            //Disable text input
            SDL_StopTextInput();
        }

    //Free resources and close SDL
    close();

    return 0;
}