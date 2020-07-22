#include "telepad.h"
#include "JUTA/JUTA_math.h"
#include "JUTA/JUTA_geometry_core.h"

#define SLIDEBAR_WIDTH 12
#define HALF_SLIDEBAR 6
#define SLIDEBAR_SEL_BLEED 5
#define TELEPAD_GRID_SIZE 60

//--------------------------------------------------------------------------------------
Telepad::Telepad(
    const size_t &master_width,
    const size_t &master_height)

{
    //Setting up dimension local varible
    win_width = master_width;
    win_height = master_height;
    pad_width = master_width / 2;
    pad_height = master_height;

    //viewport size init
    padViewport.x = 0;
    padViewport.y = 0;
    padViewport.w = pad_width- HALF_SLIDEBAR;
    padViewport.h = pad_height;

    //Slide bar size init
    slidebar.x = pad_width - HALF_SLIDEBAR;
    slidebar.y = 0;
    slidebar.w = SLIDEBAR_WIDTH;
    slidebar.h = pad_height;

    //Slide bar seletion init
    slidebarSelected = false;
    gridSelected = false;

    //panel value
    onPad = false;
    onBar = false;
    onTall = false;

    //world grid Starting Point;
    origin.x = pad_width / 2;
    origin.y = pad_height / 2;
    grid_size = TELEPAD_GRID_SIZE;

    _selecting = false;

}
/*
#[[[[[[[[[[[]]]]]]]]]]]
#[[[[[[[[[[[]]]]]]]]]]]
*/
//--------------------------------------------------------------------------------------
Telepad::~Telepad()
{
}
/*
#[[[[[[[[[[[]]]]]]]]]]]
#[[[[[[[[[[[]]]]]]]]]]]
*/
Point2D<int> *Telepad::GetOrigin()
{
    return &origin;
}
/*
#[[[[[[[[[[[]]]]]]]]]]]
#[[[[[[[[[[[]]]]]]]]]]]
*/
//--------------------------------------------------------------------------------------
void Telepad::Update(Telecontroller &controller)
{
    
    //current mouse location
    int x, y;
    SDL_GetMouseState(&x, &y);

    Resize(controller, x, y);
    MoveGrid(controller, x, y);
    
    

    if ((controller.GetCommand() == cmd_KEY::cmd_CREATE_NODE_Point) && (controller.GetCurrentPanel() == PanelID::ON_PAD))
    {
        CreateNode(x, y);
    }
    else if (controller.GetCommand() == cmd_KEY::cmd_CREATE_NODE_Point_M)
    {
        if (node_pool.empty())
        {
            CreateNode(controller.GetPadRect()->x + (controller.GetPadRect()->w / 2), controller.GetPadRect()->y + (controller.GetPadRect()->h / 2));
        }
        else
        {
            CreateNode(node_pool[node_pool.size() - 1]->GetLocation().x + origin.x + 50, node_pool[node_pool.size() - 1]->GetLocation().y + origin.y + 50);
        }
    }
    if(_selecting)
    {
        controller.Shared_Nevigation_Lock = MouseLockID::TELE_LOCKED;
        Node::ToggleGroupSelect(true);
    }else
    {
        controller.Shared_Nevigation_Lock = MouseLockID::FREE;
    }
    UpdateNode(&controller);
    Select(&controller, x, y);

    

    controller.UpdateSplitLocation(pad_width + HALF_SLIDEBAR + SLIDEBAR_SEL_BLEED);
    controller.LinkPadRect(&padViewport, &slidebar);

}
/*
#
#
#
#
*/
//--------------------------------------------------------------------------------------
void Telepad::Render(SDL_Renderer *renderer)
{
    //background region here
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_RenderFillRect(renderer, &padViewport);
    //clipping here
    SDL_RenderSetClipRect(renderer, &padViewport);
    //Draw gird---------------------------
    DrawGrid(renderer,origin,grid_size,pad_width,pad_height,70);
    //render nodes and geo's here:
    if(origin.InBound(0,0,pad_width,pad_height))
    {
        DrawCross(renderer,origin,5,110,110,110);
    }
    //..
    DrawNodes(renderer);
    DrawSelectionRect(renderer);
    DrawConnectCurve(renderer);
    //Draw slider---------------------------
    DrawSlideBar(renderer);
}


/*
#
#
#
#
#
#
*/
//------------------------------------------------------------------------------------
void Telepad::DrawSlideBar(SDL_Renderer *renderer)
{
    slidebar.x = pad_width - HALF_SLIDEBAR;
    slidebar.h = pad_height;

    SDL_RenderSetClipRect(renderer, NULL);
    SDL_SetRenderDrawColor(renderer, 130, 130, 130, 255);
    SDL_RenderFillRect(renderer, &slidebar);

    SDL_SetRenderDrawColor(renderer, 90, 90, 90, 255);
    SDL_RenderDrawLine(renderer, pad_width - HALF_SLIDEBAR, 0, pad_width - HALF_SLIDEBAR, pad_height);
    SDL_RenderDrawLine(renderer, pad_width + HALF_SLIDEBAR, 0, pad_width + HALF_SLIDEBAR, pad_height);

    SDL_RenderDrawLine(renderer, pad_width + 2, pad_height / 2 + 5, pad_width + 2, pad_height / 2 - 5);
    SDL_RenderDrawLine(renderer, pad_width, pad_height / 2 + 5, pad_width, pad_height / 2 - 5);
    SDL_RenderDrawLine(renderer, pad_width - 2, pad_height / 2 + 5, pad_width - 2, pad_height / 2 - 5);

    SDL_SetRenderDrawColor(renderer, 90, 90, 90, 255);
    SDL_RenderDrawLine(renderer, pad_width - HALF_SLIDEBAR + 1, 0, pad_width - HALF_SLIDEBAR + 1, pad_height);
    SDL_RenderDrawLine(renderer, pad_width + HALF_SLIDEBAR - 1, 0, pad_width + HALF_SLIDEBAR - 1, pad_height);
    SDL_SetRenderDrawColor(renderer, 110, 110, 110, 255);
    SDL_RenderDrawLine(renderer, pad_width - HALF_SLIDEBAR + 2, 0, pad_width - HALF_SLIDEBAR + 2, pad_height);
    SDL_RenderDrawLine(renderer, pad_width + HALF_SLIDEBAR - 2, 0, pad_width + HALF_SLIDEBAR - 2, pad_height);

    SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255);
    SDL_RenderDrawLine(renderer, pad_width - HALF_SLIDEBAR + 3, 0, pad_width - HALF_SLIDEBAR + 3, pad_height);
    SDL_RenderDrawLine(renderer, pad_width + HALF_SLIDEBAR - 3, 0, pad_width + HALF_SLIDEBAR - 3, pad_height);
}
/*
#
#
#

#
#
*/
//--------------------------------------------------------------------------------------
void Telepad::Resize(Telecontroller &controller, const int &x, const int &y)
{
    if (controller.GetCommand() == cmd_KEY::cmd_WINDOWSIZE)
    {
        double proportion = ((double)pad_width) / ((double)win_width);
        SDL_Window *hhwnd = controller.GetHWND();
        int wsX, wsY;
        SDL_GetWindowSize(hhwnd, &wsX, &wsY);

        double propxc =((double)wsX) / ((double)win_width);
        double propyc = ((double)wsY) / ((double)win_height);
        origin.x = (int)((double)origin.x*propxc);
        origin.y = (int)((double)origin.y*propyc);

        win_width = wsX;
        win_height = wsY;

        pad_width = (int)((double)wsX) * proportion;

        pad_height = wsY;

        //Updating Viewport Here
        padViewport.w = pad_width -HALF_SLIDEBAR;
        padViewport.h = pad_height;
    }

    SDL_Cursor *cursor = NULL;
    onBar = (((x > slidebar.x - SLIDEBAR_SEL_BLEED) && (x < slidebar.x + slidebar.w + SLIDEBAR_SEL_BLEED))&&(y>controller.GetPadRect()->y && y<controller.GetPadRect()->h)) && !_selecting;
    onTall = (x > slidebar.x + slidebar.w + SLIDEBAR_SEL_BLEED) && (x < win_width);


    if (onBar) //Mouse passing the slide bar
    {
        
        cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEWE);
        SDL_SetCursor(cursor);
        SDL_ShowCursor(1);
        controller.current_panel = PanelID::ON_BAR;
        //changing cursor

    }

    

    if (controller.MouseL_hold) //Mouse clicked down
    {
        if (onBar || slidebarSelected) //Hold or Grag
        {
            SDL_ShowCursor(1);
            controller.current_panel = PanelID::ON_BAR;
            cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);
            SDL_SetCursor(cursor);
            slidebarSelected = true;
            pad_width = x;
            JUTA_Math::Clamp<size_t>(pad_width, 20, win_width - 20);

            padViewport.w = pad_width -HALF_SLIDEBAR;              //Updating viewport's location
            slidebar.x = pad_width - HALF_SLIDEBAR; //updating slidebar's location
        }
    }
    else
    {
        slidebarSelected = false; //Set seleted state to false
    }
    

}
/*
#
#
*/
//-------------------------------------------------------------------------------------
void Telepad::MoveGrid(Telecontroller &controller, const int &x, const int &y)
{
    SDL_Cursor *cursor = NULL;
    onPad = ((x > 0) && (x < pad_width - HALF_SLIDEBAR))&&(y>controller.GetPadRect()->y && y<controller.GetPadRect()->h) && controller.current_panel!= PanelID::ON_MENU || controller.current_panel == PanelID::ON_PAD;
    if ((controller.MouseR_hold) && (controller.Shared_Nevigation_Lock == MouseLockID::TELE_LOCKED || controller.Shared_Nevigation_Lock == MouseLockID::FREE))
    {
        if (onPad)
        {
            controller.current_panel = PanelID::ON_PAD;
            cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);
            SDL_SetCursor(cursor);
        }

        if (mouse_trail.empty())
        {
            Point2D<int> temp(x, y);
            mouse_trail.emplace_back(std::move(temp));
        }

        if (onPad || gridSelected)
        {
            controller.current_panel = PanelID::ON_PAD;
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
            controller.Shared_Nevigation_Lock = MouseLockID::TELE_LOCKED;
        }
    }
    else if ((onPad && controller.GetCommand() == cmd_KEY::cmd_HOME)|| controller.GetCommand() == cmd_KEY::cmd_HOME_Pad)
    {
        origin.x = controller.GetPadRect()->w / 2 + controller.GetPadRect()->x;
        origin.y = controller.GetPadRect()->h / 2 + controller.GetPadRect()->y;
    }
    else if ((onPad && controller.GetCommand() == cmd_KEY::cmd_FRAME)|| controller.GetCommand() == cmd_KEY::cmd_HOME_Pad)
    {
        if (!node_pool.empty())
        {
            origin.x = controller.GetPadRect()->w / 2 + controller.GetPadRect()->x;
            origin.y = controller.GetPadRect()->h / 2 + controller.GetPadRect()->y;


            int av_x = 0, av_y = 0;
            for (auto &n : node_pool)
            {
                av_x += n->GetLocation().x + origin.x;
                av_y += n->GetLocation().y + origin.y;
            }
            av_x = (av_x/node_pool.size());
            av_y = (av_y/node_pool.size());

            

            origin.x += (controller.GetPadRect()->w / 2 + controller.GetPadRect()->x) - av_x;
            origin.y += (controller.GetPadRect()->h / 2 + controller.GetPadRect()->y) - av_y;

        }else
        {
            origin.x = controller.GetPadRect()->w / 2 + controller.GetPadRect()->x;
            origin.y = controller.GetPadRect()->h / 2 + controller.GetPadRect()->y;
        }
    }
    else
    {
        if (onPad)
        {
            controller.current_panel = PanelID::ON_PAD;
            cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
            SDL_SetCursor(cursor);
            SDL_ShowCursor(1);
        }
        if (!_selecting)
        {
            //after release set gimble location
            while (mouse_trail.size() > 0)
            {
                mouse_trail.pop_back();
            }
        }
        gridSelected = false;
        controller.Shared_Nevigation_Lock = MouseLockID::FREE;
    }
}


void Telepad::CreateNode(int x, int y)
{
    Point2D<double> loc(x - origin.x, y- origin.y);
    node_pool.emplace_back(std::make_unique<PointNode>(loc, origin, 1.0));
}

void Telepad::UpdateNode(Telecontroller* controller)
{
    if(!node_pool.empty())
    {
        for(auto& node : node_pool)
        {
            node->Update(controller,origin,1);
        }
    }
}


void Telepad::DrawNodes(SDL_Renderer* renderer)
{
    if(!node_pool.empty())
    {
        for(auto& node : node_pool)
        {
            node->DrawNode(renderer,_Icm);
        }
    }
}

void Telepad::AssignIconManager(std::shared_ptr<IconManager>& Icm)
{
    this->_Icm = Icm;
}

void Telepad::Select(Telecontroller *controller, int x, int y)
{
    int ondrags = 0;
    int onpass = 0;
    int onconnect = 0;
    for(auto& n: node_pool)
    {
        if(n->GetIfDrag())
        {
            ondrags++;
        }
        if(n->GetIfPass())
        {
            onpass++;
        }
        if(n->GetIfConnecting())
        {
            onconnect++;
        }
    }

    

    if (onPad && onconnect != 0)
    {
        _connecting = true;
    }
    else
    {
        _connecting = false;
    }

    Node::SetPassingCount(onpass);
    if(onpass == 0)
    {
        if (controller->GetCommand() == cmd_KEY::cmd_LMB)
        {
            for(auto& n: node_pool)
            {
                n->SetAsUnselected();
            }
        }
    }


    if (onPad && ondrags == 0)
    {
        if (controller->GetCommand() == cmd_KEY::cmd_LMB)
        {

            if (mouse_trail.empty())
            {
                Point2D<int> temp(x, y);
                mouse_trail.emplace_back(std::move(temp));
            }
        }

        if (controller->GetCommand() == cmd_KEY::cmd_LMB || controller->MouseL_hold)
        {
            _selecting = true;
            
            Point2D<int> temp(x, y);
            mouse_trail.emplace_back(std::move(temp));
            controller->Shared_Nevigation_Lock = MouseLockID::TELE_LOCKED;

            if (mouse_trail.size() > 3)
            {
                mouse_trail.erase(mouse_trail.begin() + 1);
                mouse_trail.erase(mouse_trail.begin() + 1);
            }

            if (!_connecting)
            {

                //Here: Making the box selection:
                for (auto &n : node_pool)
                {
                    if (mouse_trail[0].y < mouse_trail.back().y)
                    {
                        if (mouse_trail[0].x < mouse_trail.back().x)
                        {
                            if (n->GetLocation().InBound(mouse_trail[0].x - origin.x, mouse_trail[0].y - origin.y, mouse_trail.back().x - origin.x, mouse_trail.back().y - origin.y))
                            {
                                n->SetAsSelected();
                            }
                        }
                        else
                        {
                            if (n->GetLocation().InBound(mouse_trail.back().x - origin.x, mouse_trail[0].y - origin.y, mouse_trail[0].x - origin.x, mouse_trail.back().y - origin.y))
                            {
                                n->SetAsSelected();
                            }
                        }
                    }
                    else
                    {
                        if (mouse_trail[0].x > mouse_trail.back().x)
                        {
                            if (n->GetLocation().InBound(mouse_trail.back().x - origin.x, mouse_trail.back().y - origin.y, mouse_trail[0].x - origin.x, mouse_trail[0].y - origin.y))
                            {
                                n->SetAsSelected();
                            }
                        }
                        else
                        {
                            if (n->GetLocation().InBound(mouse_trail[0].x - origin.x, mouse_trail.back().y - origin.y, mouse_trail.back().x - origin.x, mouse_trail[0].y - origin.y))
                            {
                                n->SetAsSelected();
                            }
                        }
                    }
                }//end of for
            }//end if selecting
        }
        else
        {
            _selecting = false;
        }
        
    }
    else
    {
        _selecting = false;
    }
}

void Telepad::DrawSelectionRect(SDL_Renderer* renderer)
{
    if (!mouse_trail.empty() && _selecting && !_connecting)
    {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255,200);
        //SDL_RenderDrawLine(renderer, mouse_trail[0].x, mouse_trail[0].y, mouse_trail.back().x, mouse_trail[0].y);
        Point2D<int> leftcor(mouse_trail.back().x, mouse_trail[0].y);
        Point2D<int> rightcor(mouse_trail[0].x, mouse_trail.back().y);
        /*
        [0]----vh---[leftcor]
         |             |
         |             |vv
        [rightcor]----[back]
        
        */
       DrawDashLine(renderer, mouse_trail.front(), leftcor, 4);
       DrawDashLine(renderer, leftcor, mouse_trail.back(), 4);
       DrawDashLine(renderer, rightcor, mouse_trail.front(), 4);
       DrawDashLine(renderer, mouse_trail.back(), rightcor, 4);
       SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    }
}

void Telepad::DrawConnectCurve(SDL_Renderer* renderer)
{
    if (!mouse_trail.empty() &&_connecting)
    {
        std::vector<Point2D<int>> pointlist;
        pointlist.push_back(mouse_trail.front());
        Point2D<int> midpt;
        midpt.x = mouse_trail.front().x;
        midpt.y = 0.7 * mouse_trail.front().y + 0.3 * mouse_trail.back().y;
        pointlist.push_back(midpt);
        midpt.x = mouse_trail.back().x;
        midpt.y = 0.3 * mouse_trail.front().y + 0.7 * mouse_trail.back().y;
        pointlist.push_back(midpt);
        pointlist.push_back(mouse_trail.back());

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        DrawNurbs(renderer, pointlist, 3);
    }
}