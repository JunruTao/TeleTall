#ifndef CURVE_H
#define CURVE_H
#include "SDL2/SDL.h"
#include <vector>

struct Pt
{
    int x;
    int y;
};

class ZCurve
{
public:
    ZCurve():_degree(1),active(false){};
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

#endif