#include "tallwindow.h"

#define TALL_GRID_SIZE 20

Tallwindow::Tallwindow(
    const size_t &master_width,
    const size_t &master_height)
{
    //Setting up dimension local varible
    win_width = master_width;
    win_height = master_height;

    //Slide bar seletion init
    gridSelected = false;

    //world grid Starting Point;
    origin.x = win_width / 4 * 3;
    origin.y = win_height / 2;
    grid_size = TALL_GRID_SIZE;
}

Tallwindow::~Tallwindow() {}
Point2D<int> *Tallwindow::GetOrigin()
{
    return &origin;
}

void Tallwindow::Render(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
    SDL_RenderClear(renderer); //this will be the back ground for tall
    DrawGrid(renderer, origin, grid_size, win_width, win_height, 200);
    DrawGrid(renderer, origin, grid_size*10, win_width, win_height, 150);
    int cross_size = 5;
    if (origin.InBound(0,0,win_width,win_height))
    {
        DrawCross_D(renderer,origin,cross_size,220,0,0);
    }
}

void Tallwindow::Update(Telecontroller &controller)
{
    if (controller.eWinUpdate)
    {
        SDL_Window *hhwnd = controller.GetHWND();
        int wsX, wsY;

        SDL_GetWindowSize(hhwnd, &wsX, &wsY);

        double propxc =((double)wsX) / ((double)win_width);
        double propyc = ((double)wsY) / ((double)win_height);
        origin.x = (int)((double)origin.x*propxc);
        origin.y = (int)((double)origin.y*propyc);

        win_width = wsX;
        win_height = wsY;
    }
    int x, y;
    SDL_GetMouseState(&x, &y);
    MoveGrid(controller, x, y);
}

void Tallwindow::MoveGrid(const Telecontroller &controller, const int &x, const int &y)
{
    SDL_Cursor *cursor = NULL;
    if (controller.RMB_hold)
    {
        if (controller.current_panel == PanelID::ON_TALL)
        {
            cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);
            SDL_SetCursor(cursor);
        }

        if (mouse_trail.empty())
        {
            Point2D<int> temp(x, y);
            mouse_trail.emplace_back(std::move(temp));
        }

        if (controller.current_panel == PanelID::ON_TALL || gridSelected)
        {
            if (mouse_trail.size() > 1)
            {
                mouse_trail.erase(mouse_trail.begin());
            }
            Point2D<int> temp(x, y);
            mouse_trail.emplace_back(std::move(temp));
            Point2D<int> motion = mouse_trail[1] - mouse_trail[0];

            if (motion.Length() > 700)
            {
                motion.x = 0;
                motion.y = 0;
            }
            origin.Translate(motion);

            //locking the dragging mode
            gridSelected = true;
        }
    }
    else if (controller.current_panel == PanelID::ON_TALL && controller.key_HOME)
    { //HOMING here>>>
        origin.x = controller.GetSplitLocation()+(win_width-controller.GetSplitLocation())/2;
        origin.y = win_height / 2;
    }
    else
    {
        //after release set gimble location
        while (mouse_trail.size() > 0)
        {
            mouse_trail.pop_back();
        }
        gridSelected = false;
    }
}