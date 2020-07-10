#include "SDL2/SDL.h"
#include <vector>
#include "Curve.h"
#include <sstream>

#define ZIG_CURVE 1
#define SMOOTH_CURVE 3

struct Pt
{
    int x;
    int y;
};

class ZCurve
{
public:
    ZCurve() : _degree(1), active(false){};
    ZCurve(Pt start, Pt end, int degree);
    ~ZCurve() {}

    Pt GetStart();
    Pt GetEnd();
    //draw functions
    void DrawCurve(SDL_Renderer *renderer);

    bool active;

private:
    std::vector<Pt> pList;
    int _degree;
};

ZCurve::ZCurve(Pt start, Pt end, int degree) : _degree(degree)
{
    active = true;
    pList.push_back(start); // p1
    Pt mid1;
    mid1.x = (int)((((double)start.x) * 0.6f) + (((double)end.x) * 0.4f));
    mid1.y = start.y;
    pList.push_back(mid1); // p2
    Pt mid2;
    mid2.x = (int)((((double)start.x) * 0.4f) + (((double)end.x) * 0.6f));
    mid2.y = end.y;
    pList.push_back(mid2);
    pList.push_back(end);
};

void ZCurve::DrawCurve(SDL_Renderer *renderer)
{

    std::vector<Pt> tempList = pList;
    if (_degree > 1)
    {
        for (size_t j = 0; j < 3; ++j)
        {
            std::vector<Pt> tempList2;
            for (size_t i = 0; i < tempList.size() - 1; i++)
            {
                if (i == 0)
                {
                    Pt be1 = tempList[i];
                    tempList2.emplace_back(std::move(be1));
                }
                Pt oneQ;
                oneQ.x = (int)((((double)tempList[i].x) * 0.75f) + (((double)tempList[i + 1].x) * 0.25f));
                oneQ.y = (int)((((double)tempList[i].y) * 0.75f) + (((double)tempList[i + 1].y) * 0.25f));
                //tempList.erase(tempList.begin());
                tempList2.emplace_back(std::move(oneQ));
                Pt threeQ;
                threeQ.x = (int)(((double)tempList[i].x) * 0.25f + ((double)tempList[i + 1].x) * 0.75f);
                threeQ.y = (int)(((double)tempList[i].y) * 0.25f + ((double)tempList[i + 1].y) * 0.75f);
                //tempList.erase(tempList.begin());
                tempList2.emplace_back(std::move(threeQ));

                if (i == tempList.size() - 2)
                {
                    Pt be2 = tempList[i + 1];
                    tempList2.emplace_back(std::move(be2));
                }
            }
            tempList = tempList2;
        }
    }
    for (size_t i = 0; i < tempList.size() - 1; ++i)
    {
        SDL_RenderDrawLine(renderer, tempList[i].x, tempList[i].y, tempList[i + 1].x, tempList[i + 1].y);
    }
}

Pt ZCurve::GetStart()
{
    return pList[0];
}

Pt ZCurve::GetEnd()
{
    return pList[pList.size() - 1];
}

/* XPM */
static const char *arrow[] = {
    /* width height num_colors chars_per_pixel */
    "    32    32        3            1",
    /* colors */
    "X c #000000",
    ". c #ffffff",
    "  c None",
    /* pixels */
    "              XXX               ", //01
    "              X.X               ", //02
    "              X.X               ", //03
    "              X.X               ", //04
    "              X.X               ", //05
    "              X.X               ", //06
    "              X.X               ", //07
    "              X.X               ", //08
    "              X.X               ", //09
    "              XXX               ", //10
    "               X                ", //11
    "                                ", //12
    "                                ", //13
    "                                ", //14
    "XXXXXXXXXXX         XXXXXXXXXXXX", //15
    "X.........XX       XX..........X", //16
    "XXXXXXXXXXX         XXXXXXXXXXXX", //17
    "                                ", //18
    "                                ",
    "                                ",
    "               X                ",
    "              XXX               ",
    "              X.X               ",
    "              X.X               ",
    "              X.X               ",
    "              X.X               ",
    "              X.X               ",
    "              X.X               ",
    "              X.X               ",
    "              X.X               ",
    "              X.X               ",
    "              XXX               ",
    "0,0"};

static SDL_Cursor *init_system_cursor(const char *image[])
{
    int i, row, col;
    Uint8 data[4 * 32];
    Uint8 mask[4 * 32];
    int hot_x, hot_y;

    i = -1;
    for (row = 0; row < 32; ++row)
    {
        for (col = 0; col < 32; ++col)
        {
            if (col % 8)
            {
                data[i] <<= 1;
                mask[i] <<= 1;
            }
            else
            {
                ++i;
                data[i] = mask[i] = 0;
            }
            switch (image[4 + row][col])
            {
            case 'X':
                data[i] |= 0x01;
                mask[i] |= 0x01;
                break;
            case '.':
                mask[i] |= 0x01;
                break;
            case ' ':
                break;
            }
        }
    }
    sscanf(image[4 + row], "%d,%d", &hot_x, &hot_y);
    return SDL_CreateCursor(data, mask, 32, 32, hot_x + 16, hot_y + 16);
}

int main(int argc, char *argv[])
{

    bool hold = false;
    bool creation = false;

    std::vector<Pt> pList;
    if (SDL_Init(SDL_INIT_VIDEO) == 0)
    {
        SDL_Window *window = NULL;
        SDL_Renderer *renderer = NULL;
        SDL_Cursor *cur = init_system_cursor(arrow);
        SDL_SetCursor(cur);

        if (SDL_CreateWindowAndRenderer(640, 480, 0, &window, &renderer) == 0)
        {
            SDL_bool done = SDL_FALSE;
            int CURVE_MODE = SMOOTH_CURVE;

            std::vector<ZCurve> CurveList;

            while (!done)
            {
                ZCurve current;
                SDL_Delay(10);

                SDL_Event event;

                SDL_SetRenderDrawColor(renderer, 100, 100, 100, SDL_ALPHA_OPAQUE);
                SDL_RenderClear(renderer); //fill background

                SDL_PollEvent(&event);

                switch (event.type)
                {

                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym)
                    {
                    case SDLK_1:
                        CURVE_MODE = ZIG_CURVE;
                        break;
                    case SDLK_3:
                        CURVE_MODE = SMOOTH_CURVE;
                        break;
                    }
                    break;

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
                        current = ZCurve(pList[0], pList[pList.size() - 1], CURVE_MODE);
                        current.DrawCurve(renderer);
                        creation = true;
                        //SDL_RenderDrawLine(renderer, pList[0].x, pList[0].y, pList[pList.size() - 1].x, pList[pList.size() - 1].y);
                        SDL_SetRenderDrawColor(renderer, 50, 255, 255, SDL_ALPHA_OPAQUE);
                        SDL_Rect rec1, rec2;
                        rec1.x = current.GetStart().x - 5;
                        rec1.y = current.GetStart().y - 5;
                        rec1.w = 10;
                        rec1.h = 10;
                        rec2.x = current.GetEnd().x - 5;
                        rec2.y = current.GetEnd().y - 5;
                        rec2.w = 10;
                        rec2.h = 10;
                        SDL_RenderDrawRect(renderer, &rec1);
                        SDL_RenderDrawRect(renderer, &rec2);
                    }
                    break;

                case SDL_MOUSEBUTTONUP:

                    hold = false;

                    if (creation == true)
                    {
                        creation = false;
                        CurveList.emplace_back(pList[0], pList[pList.size() - 1], CURVE_MODE);
                    }

                    pList.clear();
                    break;

                default:
                    hold = false;
                    creation = false;
                    break;
                }

                if (!CurveList.empty())
                {
                    SDL_SetRenderDrawColor(renderer, 220, 220, 220, SDL_ALPHA_OPAQUE);
                    for (auto &e : CurveList)
                    {
                        e.DrawCurve(renderer);
                    }
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
