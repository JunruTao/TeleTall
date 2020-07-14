#ifndef TELEPAD_H
#define TELEPAD_H

#include <SDL2/SDL.h>
#include <vector>
#include "teletall_control.h"
#include "JUTA/JUTA_math.h"
#include "JUTA/JUTA_geometry_core.h"
#include "ui/teletall_graphics.h"
#include "ui/teletall_button.h"

class Telepad
{
public:
    Telepad(const size_t &master_width, const size_t &master_height);
    ~Telepad();

    void Update(Telecontroller &controller);
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
    bool onPad;
    bool onBar;
    bool onTall;
    
    std::vector<Point2D<int>> mouse_trail;
    
    //The grid
    Point2D<int> origin;
    int grid_size;
    void DrawSlideBar(SDL_Renderer *renderer);
    void MoveGrid(Telecontroller &controller, const int &x, const int &y);
    void Resize(Telecontroller &controller, const int &x, const int &y);

    Button* button;
};

#endif