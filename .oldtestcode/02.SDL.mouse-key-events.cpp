#include <SDL2/SDL.h>
#include <cmath>
#include <sstream>

int main(int argc, char **argv)
{
    // variables

    bool quit = false;

    SDL_Event event;

    int x = 288;
    int y = 208;
    int wd = 64;
    int ht = 80;

    bool hold = false;
    double roll = 0;
    int mouseX = 0;
    int mouseY = 0;
    int _mouseX = 0;
    int _mouseY = 0;
    int delX = 0;
    int delY = 0;

    // init SDL

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("SDL2 Keyboard/Mouse events",
                                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_Surface *image = SDL_LoadBMP("a.bmp");
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer,
                                                        image);
    SDL_FreeSurface(image);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // handle events

    while (!quit)
    {
        SDL_Delay(1);
        SDL_PollEvent(&event);
        std::stringstream ss;

        switch (event.type)
        {
        case SDL_QUIT:
            quit = true;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_LEFT:
                x--;
                break;
            case SDLK_RIGHT:
                x++;
                break;
            case SDLK_UP:
                y--;
                break;
            case SDLK_DOWN:
                y++;
                break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            switch (event.button.button)
            {
            case SDL_BUTTON_LEFT:
                hold = false;
                break;
            case SDL_BUTTON_RIGHT:
                hold = true;
                break;
            default:
                hold = true;
                break;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            hold = false;
            break;

        case SDL_MOUSEMOTION:
            mouseX = event.motion.x;
            mouseY = event.motion.y;

            if (hold)
            {
                delX = mouseX - _mouseX;
                delY = mouseY - _mouseY;
                x += delX;
                y += delY;
            }

            ss << "X: " << mouseX << " Y: " << mouseY;
            SDL_SetWindowTitle(window, ss.str().c_str());

            break;
        }

        
        SDL_Rect dstrect = {x, y, wd, ht};

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, &dstrect);
        SDL_RenderPresent(renderer);

        _mouseX = mouseX;
        _mouseY = mouseY;
        roll = 0;
    }

    // cleanup SDL

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}