#ifndef TALLWINDOW_H
#define TALLWINDOW_H

#include <SDL2/SDL.h>
#include "teletall_control.h"
#include "JUTA/JUTA_geometry_core.h"
#include "JUTA/JUTA_math.h"
#include <vector>

class Tallwindow
{
public:
    Tallwindow(
        const size_t &master_width,
        const size_t &master_height);
    ~Tallwindow();

    void Update(const Telecontroller &controller);

    void Render(SDL_Renderer *renderer);
    Point2D<int>* GetOrigin();

private:
    size_t win_width;
    size_t win_height;
    size_t pad_width;
    size_t pad_height;

    bool gridSelected;

    //The grid
    Point2D<int> origin;
    std::vector<Point2D<int>> mouse_trail; 
    int grid_size;

    void DrawGrid(SDL_Renderer *renderer);
    void MoveGrid(const Telecontroller &controller, const int &x, const int &y);

};

#endif