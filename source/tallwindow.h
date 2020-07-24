#ifndef TALLWINDOW_H
#define TALLWINDOW_H



#include "x_sdl2headers.h"
#include "teletall_control.h"
#include "telenode.h"
#include "JUTA/JUTA_geometry_core.h"
#include "JUTA/JUTA_math.h"
#include "ui/teletall_graphics.h"
#include <vector>


class Tallwindow
{
public:
    Tallwindow(
        const size_t &master_width,
        const size_t &master_height);
    ~Tallwindow();

    void CaptureRenderNodes(const std::shared_ptr<Node>& node);
    void CaptureEditNode(const std::shared_ptr<Node>& node);
    void Update(Telecontroller &controller);

    void Render(SDL_Renderer *renderer);
    Point2D<int>* GetOrigin();

private:
    size_t win_width;
    size_t win_height;

    bool gridSelected;
    bool gridSnap;
    bool onTall;
    bool mouseDown;

    //node that drawing:
    std::vector<std::shared_ptr<Node>> displaying_nodes;
    std::shared_ptr<Node> editing_node;

    //The grid
    Point2D<int> origin;
    Point2D<int>* mouseLocation_ptptr; //Pointer here!! delete after please
    std::vector<Point2D<int>> mouse_trail; 
    SDL_Rect tallviewport_RECT;
    int grid_size;
    Point2D<float> CalcLocalCord(const Point2D<int>& origin, const Point2D<int>& target);

    void MoveGrid(Telecontroller &controller, const int &x, const int &y);
    void EditGeometry(Telecontroller *controller, const int &x, const int &y);

    ScreenText* cordText;
    SDL_Color text_color;

};

#endif