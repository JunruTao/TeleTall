#include "SDL2/SDL.h"
#include <vector>

#define POINTS_COUNT 4

static SDL_Point points[POINTS_COUNT] = {
    {320, 200},
    {300, 240},
    {340, 240},
    {320, 200}};

struct Pt
{
    int x;
    int y;
};

int main(int argc, char *argv[])
{

    bool hold = false;
    std::vector<Pt> pList;
    if (SDL_Init(SDL_INIT_VIDEO) == 0)
    {
        SDL_Window *window = NULL;
        SDL_Renderer *renderer = NULL;

        if (SDL_CreateWindowAndRenderer(640, 480, 0, &window, &renderer) == 0)
        {
            SDL_bool done = SDL_FALSE;

            while (!done)
            {
                SDL_Event event;

                SDL_SetRenderDrawColor(renderer, 100, 100, 100, SDL_ALPHA_OPAQUE);
                SDL_RenderClear(renderer); //fill background

                SDL_PollEvent(&event);

                switch (event.type)
                {
                case SDL_MOUSEBUTTONDOWN:

                    switch (event.button.button)
                    {
                    case SDL_BUTTON_LEFT:

                        if (pList.empty())
                        {
                            Pt point;
                            point.x = event.motion.x;
                            point.y = event.motion.y;
                            pList.push_back(point);
                            hold = true;
                        }
                        break;

                    default:
                        hold = false;
                        break;
                    }
                    break;

                case SDL_MOUSEMOTION:
                    if (hold)
                    {
                        if (pList.size() > 1)
                        {
                            pList.pop_back();
                        }
                        Pt point;
                        point.x = event.motion.x;
                        point.y = event.motion.y;
                        pList.push_back(point);
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
                        
                        SDL_RenderDrawLine(renderer, pList[0].x, pList[0].y, pList[pList.size() - 1].x, pList[pList.size() - 1].y);
                        

                        //pList.erase(pList.begin());
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    hold = false;
                    pList.clear();
                    break;

                default:
                    hold = false;
                    break;
                }

                SDL_RenderPresent(renderer);
                if (event.type == SDL_QUIT)
                {
                    done = SDL_TRUE;
                }
            }
        }

        if (renderer)
        {
            SDL_DestroyRenderer(renderer);
        }
        if (window)
        {
            SDL_DestroyWindow(window);
        }
    }

    SDL_Quit();
    return 0;
}
