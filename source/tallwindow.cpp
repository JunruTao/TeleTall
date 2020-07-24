#include "tallwindow.h"
#include "JUTA/JUTA_math.h"

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
    cordText = new ScreenText();
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
    SDL_SetRenderDrawColor(renderer, 190, 190, 190, 255);//<|==================={Background color}
    SDL_RenderClear(renderer); //this will be the back ground for tall

 
    DrawGrid(renderer, origin, grid_size, win_width, win_height, 170);//<|==================={Small Grid color color}
    DrawGrid(renderer, origin, grid_size*10, win_width, win_height, 120);//<|==================={Times-x10 Grid color}
    
    int cross_size = 15;
    if (origin.InBound(0,0,win_width,win_height))
    {
        DrawCross_D(renderer,origin,cross_size,100,100,100);
    }


    
    
    if(onTall) // Window Graphics: inrange rec and mouse cross
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200);

        DrawLineExtendToBound(renderer,mouseLocation_ptptr,&tallviewport_RECT,1);//draw the mouse extended line -y
        DrawLineExtendToBound(renderer,mouseLocation_ptptr,&tallviewport_RECT,0);//draw the mouse extended line -x

        DrawCross_D(renderer,*mouseLocation_ptptr,20, 255, 255, 255);
        SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_NONE);

        //send mouse cords to screen
        Point2D<float> temp = CalcLocalCord(origin,*mouseLocation_ptptr);
        std::string cortostring = "[x=" + JUTA_Math::Num_To_String_Percision<float>(temp.x,2) +", y=" + JUTA_Math::Num_To_String_Percision<float>(temp.y,2)+ "]";
        
        //Drawing the world coordinates on tall mouse
        cordText->loadFromRenderedText(cortostring,renderer,text_color);
        cordText->Draw(renderer,mouseLocation_ptptr->x +3,mouseLocation_ptptr->y+3);

        
    }
    if (!nodes.empty())
    {
        for (auto &n : nodes)
        {
            n->DrawGeometry(renderer, origin, grid_size);
        }
        nodes.clear();
    }
    
}



//-----------------------------------------------
void Tallwindow::Update(Telecontroller &controller)
{
    //Handling Window Resizing Events
    if (controller.GetCommand() == cmd_KEY::cmd_WINDOWSIZE)
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

    controller.LinkTallRec(&tallviewport_RECT);

}

void Tallwindow::MoveGrid(Telecontroller &controller, const int &x, const int &y)
{

    Point2D tempP(x, y);
    onTall = (tempP.InBoundWH(controller.GetTallRect()->x, controller.GetTallRect()->y, controller.GetTallRect()->w, controller.GetTallRect()->h)) || controller.current_panel == PanelID::ON_TALL;
    if (onTall)
    {
        controller.current_panel = PanelID::ON_TALL;
        SDL_ShowCursor(0);
    }
    else
    {
        SDL_ShowCursor(1);
    }
    if (controller.MouseR_hold)
    {
        if (mouse_trail.empty())
        {
            Point2D<int> temp(x, y);
            mouse_trail.emplace_back(std::move(temp));
        }

        if ((controller.Shared_Nevigation_Lock == MouseLockID::TALL_LOCKED || controller.Shared_Nevigation_Lock == MouseLockID::FREE) && (onTall || gridSelected))
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
            controller.Shared_Nevigation_Lock = MouseLockID::TALL_LOCKED;
        }
    }
    else if ((onTall && controller.GetCommand() == cmd_KEY::cmd_HOME) || controller.GetCommand() == cmd_KEY::cmd_HOME_Tall)
    { //HOMING here>>>
        origin.x = controller.GetTallRect()->w / 2 + controller.GetTallRect()->x;
        origin.y = controller.GetTallRect()->h / 2 + controller.GetTallRect()->y;
    }
    else
    {
        //after release set gimble location
        while (mouse_trail.size() > 0)
        {
            mouse_trail.pop_back();
        }
        gridSelected = false;
        controller.Shared_Nevigation_Lock = MouseLockID::FREE;
    }
}


Point2D<float> Tallwindow::CalcLocalCord(const Point2D<int>& origin, const Point2D<int>& target)
{
    float x = -((float)(origin.x -target.x))/((float)grid_size);
    float y = +((float)(origin.y -target.y))/((float)grid_size);
    return Point2D<float>(x,y);
}


void Tallwindow::CaptureRenderNodes(const std::shared_ptr<Node>& node)
{
    nodes.push_back(node);
}