#include "tallwindow.h"

#define TALL_GRID_SIZE 20

Tallwindow::Tallwindow(
    const size_t &master_width,
    const size_t &master_height)
{
    //Setting up dimension local varible
    win_width = master_width;
    win_height = master_height;
    tallviewport_RECT.x = 0;
    tallviewport_RECT.y = 0;
    tallviewport_RECT.h = win_height;
    tallviewport_RECT.w = win_width;

    //panel values
    onTall = false;

    //Slide bar seletion init
    gridSelected = false;

    //world grid Starting Point;
    origin.x = win_width / 4 * 3;
    origin.y = win_height / 2;
    grid_size = TALL_GRID_SIZE;

    //mouse_reference;
    mouseLocation_ptptr = new Point2D<int>();
    cordText = new ScreenText(6);
    text_color = {255,255,255,200};
}


//[[[[[[[Free Resources]]]]]]]
Tallwindow::~Tallwindow()
 {
     delete mouseLocation_ptptr;
     delete cordText;
 }



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
    
    int cross_size = 15;
    if (origin.InBound(0,0,win_width,win_height))
    {
        DrawCross_D(renderer,origin,cross_size,175,175,175);
    }


    
    if(onTall) // Window Graphics: inrange rec and mouse cross
    {
        SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 153, 153, 210);
        SDL_RenderDrawRect(renderer,&tallviewport_RECT);


        //SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_ADD);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200);
        DrawLineExtendToBound(renderer,mouseLocation_ptptr,&tallviewport_RECT,1);
        DrawLineExtendToBound(renderer,mouseLocation_ptptr,&tallviewport_RECT,0);
        DrawCross_D(renderer,*mouseLocation_ptptr,20, 255, 255, 255);
        SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_NONE);

        //send mouse cords to screen
        Point2D temp = CalcLocalCord(origin,*mouseLocation_ptptr);
        std::string cortostring = "[" + std::to_string(temp.x) +"," + std::to_string(temp.y)+ "]";
        
        cordText->loadFromRenderedText(cortostring,text_color,text_color,renderer,0);
        cordText->Draw(renderer,mouseLocation_ptptr->x +3,mouseLocation_ptptr->y+3);
    }
    
}



//-----------------------------------------------
void Tallwindow::Update(Telecontroller &controller)
{
    //Handling Window Resizing Events
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

    //moving Grid
    int x, y;
    SDL_GetMouseState(&x, &y);
    mouseLocation_ptptr->New(x,y);

    MoveGrid(controller, x, y);

    tallviewport_RECT.x = controller.GetSplitLocation() - 4;
    tallviewport_RECT.w = win_width-controller.GetSplitLocation()+4;
    tallviewport_RECT.h = win_height;

    

}

void Tallwindow::MoveGrid(const Telecontroller &controller, const int &x, const int &y)
{
    onTall = (controller.current_panel == PanelID::ON_TALL);

    SDL_Cursor *cursor = NULL;
    if (controller.RMB_hold)
    {
        if (mouse_trail.empty())
        {
            Point2D<int> temp(x, y);
            mouse_trail.emplace_back(std::move(temp));
        }

        if (onTall || gridSelected)
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
    else if (onTall && controller.key_HOME)
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


Point2D<float> Tallwindow::CalcLocalCord(const Point2D<int>& origin, const Point2D<int>& target)
{
    float x = -((float)(origin.x -target.x))/((float)grid_size);
    float y = +((float)(origin.y -target.y))/((float)grid_size);
    return Point2D<float>(x,y);
}