#ifndef TELEPAD_H
#define TELEPAD_H

#include <SDL2/SDL.h>
#include <vector>
#include "teletall_control.h"
#include "JUTA/JUTA_math.h"
#include "JUTA/JUTA_geometry_core.h"

class Telepad
{
public:
    Telepad(const size_t &master_width, const size_t &master_height);
    ~Telepad();

    void Update(const Telecontroller &controller);
    void Render(SDL_Renderer *renderer);
    Point2D<int>* GetOrigin();

private:
    size_t win_width;
    size_t win_height;
    size_t pad_width;
    size_t pad_height;
    SDL_Rect padViewport;
    SDL_Rect slidebar;
    bool slidebarSelected;
    bool gridSelected;

    //The grid
    Point2D<int> origin;
    std::vector<Point2D<int>> mouse_trail; 
    int grid_size;

    void DrawSlideBar(SDL_Renderer *renderer);
    void DrawGrid(SDL_Renderer *renderer);
    void MoveGrid(const Telecontroller &controller, const int &x, const int &y);
    void Resize(const Telecontroller &controller, const int &x, const int &y);
};

#endif