#include "telenode.h"


void IconManager::LoadIcons(SDL_Renderer* renderer)
{
    std::string icon_dir = "tltl_node_icon/"; 
    std::vector<std::string> names = 
    {
        "pointnode"
    };

    for(auto e : names)
    {
        std::string path = icon_dir + e + ".bmp";
        SDL_Surface *image = SDL_LoadBMP(path.c_str());
        
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);
        SDL_FreeSurface(image);
        map_Icons[e] = texture;
    }

}

IconManager::IconManager(SDL_Renderer* renderer)
{
    this->LoadIcons(renderer);
}

IconManager::~IconManager()
{
    std::for_each(map_Icons.begin(), map_Icons.end(), [](std::pair<std::string, SDL_Texture*> e)
    {
        SDL_DestroyTexture(e.second);
    });
}


SDL_Texture* IconManager::QueryIcon(std::string _icon_name)
{
    if(map_Icons.find(_icon_name) != map_Icons.end())
    {
        return map_Icons[_icon_name];
    }else
    {
        return nullptr;
    }
}


void IconManager::DrawIcon(SDL_Renderer* renderer, SDL_Rect* rect, std::string icon_name)
{
    SDL_RenderCopy(renderer,map_Icons[icon_name],NULL, rect);
}



//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++== NODE  CONNECTOR==+++++++++++++++++++++++++++++++++++++
//_____________________________________________________________________________________
SDL_Color NodeConnector::_norcolor = {200,200,200,255}; //dot color - normal
SDL_Color NodeConnector::_selcolor = {120,178,255,255}; //dot color - selected
SDL_Color NodeConnector::_passcolor = {240,240,240,255}; //dot color - pass
int NodeConnector::_size = 8; // dot size;



void NodeConnector::UpdateLocation(Point2D<double> location)
{
    _pos = location;
    if (_in_or_out)
    {
        _pos.y -= (_size/2 +3);
    }
    else
    {
        _pos.y += (_size/2 +3);
    }
    _rect.x = (int)(_pos.x - (_size/2));
    _rect.y = (int)(_pos.y - (_size/2));
    _rect.w = _size;
    _rect.h = _size;
}



NodeConnector::NodeConnector(Point2D<double> location, bool in_or_out, Node *parent) : _in_or_out(in_or_out),
                                                                                       _selected(false),
                                                                                       _passing(false),
                                                                                       _connecting_mode(false),
                                                                                       _parent(parent),
                                                                                       _target(nullptr)
{
    UpdateLocation(location);

}


void NodeConnector::Update(Telecontroller* controller, Point2D<double> location)
{
    bool inbound = controller->GetMousePoint()->InBoundWH(_rect.x -3,_rect.y -3,_rect.w +6,_rect.h+6);
    UpdateLocation(location);
    if(_selected && controller->MouseL_hold)
    {
        _connecting_mode = true;
        _selected = true;
        goto skip;
    }else
    {
        _connecting_mode = false;
        _selected = false;
    }


    if(inbound)
    {
        _passing = true;
        if(controller->GetCommand() == cmd_KEY::cmd_LMB)
        {
            _selected = true;
            _connecting_mode = true;
        }else
        {
            _selected = false;
        }
    }
    else
    {
        _passing = false;
    }
    skip:
    {}
}


NodeConnector::~NodeConnector()
{
    _parent = nullptr;
    _target = nullptr;
    
}

void NodeConnector::Draw(SDL_Renderer *renderer)
{
    if (!_passing)
    {
        SDL_SetRenderDrawColor(renderer, _norcolor.r, _norcolor.g, _norcolor.b, _norcolor.a);
        DrawDot8x8(renderer);
    }
    else
    {
        if (_selected)
        {
            SDL_SetRenderDrawColor(renderer, _selcolor.r, _selcolor.g, _selcolor.b, _selcolor.a);
            DrawDot8x8(renderer);
        }
        else
        {
            SDL_SetRenderDrawColor(renderer, _passcolor.r, _passcolor.g, _passcolor.b, _passcolor.a);
            DrawDot8x8(renderer);
        }
    }
}

void NodeConnector::DrawDot8x8(SDL_Renderer *renderer)
{
    SDL_RenderDrawLine(renderer, _rect.x + 2, _rect.y, _rect.x + 5, _rect.y);
    SDL_RenderDrawLine(renderer, _rect.x + 1, _rect.y + 1, _rect.x + 6, _rect.y + 1);
    SDL_RenderDrawLine(renderer, _rect.x, _rect.y + 2, _rect.x + 7, _rect.y + 2);
    SDL_RenderDrawLine(renderer, _rect.x, _rect.y + 3, _rect.x + 7, _rect.y + 3);
    SDL_RenderDrawLine(renderer, _rect.x, _rect.y + 4, _rect.x + 7, _rect.y + 4);
    SDL_RenderDrawLine(renderer, _rect.x, _rect.y + 5, _rect.x + 7, _rect.y + 5);
    SDL_RenderDrawLine(renderer, _rect.x + 1, _rect.y + 6, _rect.x + 6, _rect.y + 6);
    SDL_RenderDrawLine(renderer, _rect.x + 2, _rect.y + 7, _rect.x + 5, _rect.y + 7);
}





//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++== N O D E ==++++++++++++++++++++++++++++++++++++++++++
//_____________________________________________________________________________________
//* Node class -static members
int Node::_node_width = 100;
int Node::_node_height = 30;
int Node::_icon_edge_len = 24;
size_t Node::selected_count = 0;
size_t Node::passing_count = 0;
size_t Node::node_counter = 0;

bool Node::if_groupsel = false;
bool Node::is_editing_mode = false;
SDL_Color Node::_node_color = {100, 100, 100, 255};
SDL_Color Node::_selcolor = {255,255,0,255}; //outline
SDL_Color Node::_passcolor = {200,200,200,255}; //outline
SDL_Color Node::_textcolor = {120,120,120,255};

SDL_Color Node::_editmode_color = {255,42,105,100};
SDL_Color Node::_lockmode_color = {32,32,32,180};
SDL_Color Node::_renderflag_color = {0,80,255,180};

void Node::ProcessUserInputs(Telecontroller *controller, const Point2D<int> origin_s, double scale)
{
    bool old_sel_status = _selected;
    bool inboud = controller->GetMousePoint()->InBoundWH(_node_rect.x, _node_rect.y, _node_rect.w, _node_rect.h);

    if(controller->GetEditMode())
    {
        goto skip;
    }

    if (inboud)
    {
        _passing = true;
    }
    else
    {
        _passing = false;
    }

    //command from panel
    if(controller->GetCommand() == cmd_KEY::cmd_CLEAR_Sel)
    {   
        goto skip;
    }

    if(inboud)
    {    
        if(controller->GetCommand() == cmd_KEY::cmd_LMB  || _selected)
        {
            _selected = true;
            
            if (controller->MouseL_hold && controller->Shared_Nevigation_Lock != MouseLockID::TELE_LOCKED)
            {
                dragging:
                _ondrag = true;
                _center.x +=  (controller->GetMousePoint()->x - origin_s.x) - _clicked_old_pos.x;
                _center.y +=  (controller->GetMousePoint()->y - origin_s.y) - _clicked_old_pos.y;
            }
            else
            {
                _ondrag = false;
            }
        }
    }
    else if (_ondrag)
    {
        if (controller->MouseL_hold)
        {
            _ondrag = true;
            _center.x += (controller->GetMousePoint()->x - origin_s.x) - _clicked_old_pos.x;
            _center.y += (controller->GetMousePoint()->y - origin_s.y) - _clicked_old_pos.y;

        }
        else
        {
            _ondrag = false;
        }
    }
    else
    {
        if (if_groupsel && selected_count > 1 && _selected && passing_count > 0 && controller->MouseL_hold && controller->Shared_Nevigation_Lock != MouseLockID::TELE_LOCKED)
        {
            goto dragging;
        }

        _ondrag = false;
        
        if((controller->GetCommand() == cmd_KEY::cmd_LMB && controller->current_panel == PanelID::ON_PAD)) 
        {   
            _selected = false;
            if_groupsel = false;
        }

     
    }


    skip:{}
        if(controller->GetCommand() == cmd_KEY::cmd_CLEAR_Sel)
    {
        _selected = false;
        if_groupsel = false;
    }


    _clicked_old_pos.x = (controller->GetMousePoint()->x - origin_s.x);
    _clicked_old_pos.y = (controller->GetMousePoint()->y - origin_s.y);
}


std::shared_ptr<NodeConnector> Node::GetSelConnector()
{
    return _current_sel_connector;
}
/*================================================================================================
|
|   (O)--x> (O's screen space)
|    |                      ^
|    y                   [__i__]
|Pn(X,Y) I<_____________Node Wdith_____________>I__
|        | Pi(x,y)_I<_____Icon W._____>I        |^
|        |        ^|       _|_Cen(x,y) |        | Node Height
|        | Icon H.v|________|__________|        |
|        |______________________________________|v_ 
|                        [__o__]
|                           v
|        (relative) Pn = << Cen.x - (NodeHeight / 2), Cen.y - (NodeWidth / 2) >>
|        (screen) Pn = << (Cen.x + O.x) - (NodeHeight / 2), (Cen.y + O.y) - (NodeWidth / 2)  >>
|
|==================================================================================================*/

//takes the origin and transform back to local coordinates
void Node::ScreenTransform(const Point2D<int>& origin_s, double scale)
{
    _node_rect.x = (_center.x * scale + origin_s.x) - (scale * _node_width/2);
    _node_rect.y = (_center.y * scale + origin_s.y) - (scale * _node_height/2);
    _node_rect.w = scale * _node_width;
    _node_rect.h = scale * _node_height;

    _icon_rect.x = (_center.x * scale + origin_s.x) - (scale * _icon_edge_len/2);
    _icon_rect.y = (_center.y * scale + origin_s.y) - (scale * _icon_edge_len/2);
    _icon_rect.w = scale * _icon_edge_len;
    _icon_rect.h = scale * _icon_edge_len;
}

//constructor for all node objects
Node::Node()
{
    node_counter++;
    _selected = false;
    _passing = false;
    _running = false;
    _ondrag = false;
    _dragconnect_mode = false;
    _displaying = false;
    _editing = false;


    if(node_counter == 1)
    {
        _displaying = true;
    }
}

Node::~Node() //virtual destructor
{
    node_counter--;
}

void Node::DrawDisplayRects(SDL_Renderer* renderer, std::string icon_name, std::shared_ptr<IconManager> Icm)
{
    SDL_SetRenderDrawColor(renderer, _node_color.r, _node_color.g, _node_color.b, _node_color.a);
    SDL_RenderFillRect(renderer,&_node_rect);

    SDL_SetRenderDrawColor(renderer,_node_color.r + 20 ,_node_color.g + 20 ,_node_color.b + 20,_node_color.a);
    SDL_RenderDrawRect(renderer,&_node_rect);
    SDL_SetRenderDrawColor(renderer,_node_color.r - 12 ,_node_color.g - 12 ,_node_color.b - 12,_node_color.a);
    SDL_RenderDrawLine(renderer, _node_rect.x+1,_node_rect.y+1,_node_rect.x+_node_rect.w-2,_node_rect.y+1);
    SDL_RenderDrawLine(renderer, _node_rect.x+1, _node_rect.y+_node_rect.h -2,_node_rect.x+_node_rect.w-2,_node_rect.y+_node_rect.h -2);



    Icm->DrawIcon(renderer,&_icon_rect, icon_name);
    _textdisplay->loadFromRenderedText(_name, renderer, _textcolor, _node_color, 3);
    _textdisplay->Draw(renderer,_node_rect.x + _node_width+3, _node_rect.y + (_node_rect.h/2) - (ScreenText::GetUniversalTextHeight()/2));


    if(_displaying)
    {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);
        SDL_SetRenderDrawColor(renderer, _renderflag_color.r, _renderflag_color.g, _renderflag_color.b, 170);
        SDL_Rect flagrec = _node_rect;
        flagrec.x += _node_rect.w*0.9 - 2;
        flagrec.w = _node_rect.w*0.1-1;
        flagrec.y += 5;
        flagrec.h -= 10;
        SDL_RenderFillRect(renderer, &flagrec);
        SDL_SetRenderDrawColor(renderer, _renderflag_color.r, _renderflag_color.g, _renderflag_color.b, 180);
        SDL_RenderDrawRect(renderer, &flagrec);
        flagrec.x += 1;
        flagrec.w -= 2;
        flagrec.y += 1;
        flagrec.h -= 2;
        SDL_SetRenderDrawColor(renderer, _renderflag_color.r, _renderflag_color.g, _renderflag_color.b, 40);
        SDL_RenderDrawRect(renderer, &flagrec);
        

        flagrec.x -= 2;
        flagrec.w += 4;
        flagrec.y -= 2;
        flagrec.h += 4;

        SDL_SetRenderDrawColor(renderer, _renderflag_color.r, _renderflag_color.g, _renderflag_color.b, 100);
        SDL_RenderDrawRect(renderer, &flagrec);
        SDL_RenderDrawRect(renderer, &_node_rect);

        flagrec.x -= 1;
        flagrec.w += 2;
        flagrec.y -= 1;
        flagrec.h += 2;
        SDL_SetRenderDrawColor(renderer, _renderflag_color.r, _renderflag_color.g, _renderflag_color.b, 50);
        SDL_RenderDrawRect(renderer, &flagrec);

        flagrec.x -= 1;
        flagrec.w += 2;
        flagrec.y -= 1;
        flagrec.h += 2;
        SDL_SetRenderDrawColor(renderer, _renderflag_color.r, _renderflag_color.g, _renderflag_color.b, 20);
        SDL_RenderDrawRect(renderer, &flagrec);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    }

    if (is_editing_mode && _editing)
    {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);
        SDL_SetRenderDrawColor(renderer, _editmode_color.r, _editmode_color.g, _editmode_color.b, _editmode_color.a);
        SDL_RenderFillRect(renderer, &_node_rect);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        SDL_SetRenderDrawColor(renderer, 204, 0, 102, 255);
            SDL_RenderDrawRect(renderer, &_node_rect);
    }
    else if (is_editing_mode && !_editing)
    {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, _lockmode_color.r, _lockmode_color.g, _lockmode_color.b, _lockmode_color.a);
        SDL_RenderFillRect(renderer, &_node_rect);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    }
    else
    {
        if (_passing && !_selected)
        {
            SDL_SetRenderDrawColor(renderer, _passcolor.r, _passcolor.g, _passcolor.b, _passcolor.a);
            SDL_RenderDrawRect(renderer, &_node_rect);
        }
        else if (_selected)
        {
            SDL_SetRenderDrawColor(renderer, _selcolor.r, _selcolor.g, _selcolor.b, _selcolor.a);
            SDL_RenderDrawRect(renderer, &_node_rect);
        }
        else if (_dragconnect_mode)
        {
            SDL_SetRenderDrawColor(renderer, 0, 120, 255, _selcolor.a);
            SDL_RenderDrawRect(renderer, &_node_rect);
        }
    }
}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++== POINT NODE ==++++++++++++++++++++++++++++++++++++++++++
//_____________________________________________________________________________________
/*===================================================================================================

   (O)--x> (O's screen space)
    |                      ^
    y                   [__i__]
Pn(X,Y) I<_____________Node Wdith_____________>I__
        | Pi(x,y)_I<_____Icon W._____>I        |^
        |        ^|       _|_Cen(x,y) |        | Node Height
        | Icon H.v|________|__________|        |
        |______________________________________|v_ 
                        [__o__]
                           v
        (relative) Pn = << Cen.x - (NodeWidth / 2), Cen.y - (NodeHeight / 2) >>
        (screen) Pn = << (Cen.x + O.x) - (NodeWidth / 2), (Cen.y + O.y) - (NodeHeight / 2)  >>

===================================================================================================
*/

size_t PointNode::counter = 0;
//constructor:
PointNode::PointNode(Point2D<double> drop_location, const Point2D<int>& origin_s, double scale)
{
    counter++;

    _editable = true;
    _editing = false;
    pt_onlysel = false;


    _name = "Point" + std::to_string(counter);
    _center = drop_location;
    _clicked_old_pos = _center;

    Point2D<double> input_cloc(_center.x + origin_s.x, _center.y + (_node_height/2) + origin_s.y);
    Point2D<double> output_cloc(_center.x + origin_s.x, _center.y - (_node_height/2) + origin_s.y);
    
    //Creating input buttons
    _inputs.emplace_back(std::make_shared<NodeConnector>(input_cloc,true, this));
    _outputs.emplace_back(std::make_shared<NodeConnector>(output_cloc,false, this));

    ScreenTransform(origin_s,scale);
    _textdisplay = std::make_unique<ScreenText>();
    
}








void PointNode::Update(Telecontroller *controller, const Point2D<int> origin_s, double scale)
{
    
    ProcessUserInputs(controller,origin_s,scale);
    ScreenTransform(origin_s,scale);



    Point2D<double> input_cloc(_center.x + origin_s.x, _center.y-(_node_height/2) + origin_s.y);
    Point2D<double> output_cloc(_center.x + origin_s.x, _center.y+(_node_height/2) + origin_s.y);


    for(auto& n:_inputs)
    {
        n->Update(controller,input_cloc);
    }
    for(auto& n:_outputs)
    {
        n->Update(controller,output_cloc);
    }

    int connecting = 0;
    for(auto& n : _inputs)
    {  
        if(n->GetOnConnectDragMode())
        {
            connecting++;
            _current_sel_connector = n;
        }
    }
    for(auto& n : _outputs)
    {  
        if(n->GetOnConnectDragMode())
        {
            connecting++;
            _current_sel_connector = n;
        }
    }
    
    if(connecting>0)
    {
        _dragconnect_mode = true;
    }else
    {
        _dragconnect_mode = false;
    }

    if (controller->GetEditMode())
    {
        is_editing_mode = true;

        if (_selected && controller->GetSeletedNodesCount() == 1)
        {
            _editing = true;           
        }
        else
        {
            _editing = false;
        }
    }
    else
    {
        is_editing_mode = false;
        _editing = false;
    }
}




void PointNode::DrawNode(SDL_Renderer * renderer, std::shared_ptr<IconManager> Icm)
{

    DrawDisplayRects(renderer, "pointnode", Icm);


    for(auto& n:_inputs)
    {
        n->Draw(renderer);
    }
    for(auto& n:_outputs)
    {
        n->Draw(renderer);
    }
}


void PointNode::DrawGeometry(SDL_Renderer* renderer, Point2D<int>& origin_s, int grid_size) const 
{
    if(!point_pool.empty())
    {
        for(auto& p : point_pool)
        {
            if (_editing)
            {
                p->Draw(renderer, origin_s, grid_size, 2);
                
            }
            else
            {

                if (_displaying)
                {
                    p->Draw(renderer, origin_s, grid_size, 1);
                }
                else
                {
                    p->Draw(renderer, origin_s, grid_size, 0);
                }
            }
        }
        if(_editing && point_pool.size()>1)
        {
            for(size_t i = 0; i< point_pool.size()-1; ++i)
            {
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 130);
                DrawDashLine(renderer, point_pool[i + 1]->GetScreenLocation(origin_s, grid_size), point_pool[i]->GetScreenLocation(origin_s, grid_size), 7);
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
            }
        }
    }
}

void PointNode::ProcessInput(Telecontroller *controller, const Point2D<int> origin, int grid_size)
{
    if (controller->GetCurrentPanel() == PanelID::ON_TALL)
    {
        if (!point_pool.empty())
        {
            for (auto &p : point_pool)
            {
                if ((p->IsSeleted() ||controller->GetMousePoint()->InBound(p->GetScreenLocation(origin, grid_size).x - 5, p->GetScreenLocation(origin, grid_size).y - 5, p->GetScreenLocation(origin, grid_size).x + 5, p->GetScreenLocation(origin, grid_size).y + 5))&&!pt_onlysel)
                {
                    p->SetAsPassing();
                    
                    if ((controller->MouseL_hold)&& !controller->xkey_hold)
                    {
                        p->SetAsSelected();
                        pt_onlysel = true;
                        p->SetLocation(-((double)(origin.x - controller->GetMousePoint()->x)) / ((double)grid_size), ((double)(origin.y - controller->GetMousePoint()->y)) / ((double)grid_size), 0);
                    }
                    else if ((controller->MouseL_hold)&&controller->xkey_hold)
                    {
                        p->SetAsSelected();
                        pt_onlysel = true;
                        p->SetLocation(round(-((double)(origin.x - controller->GetMousePoint()->x)) / ((double)grid_size)), round(((double)(origin.y - controller->GetMousePoint()->y)) / ((double)grid_size)), 0);
                    }
                    else
                    {
                        p->SetAsUnselected();
                        pt_onlysel = false;
                    }
                }
                else
                {
                    p->SetAsNormal();
                }
            }
            pt_onlysel = false;
        }

        switch (controller->GetCommand())
        {
        case cmd_KEY::cmd_CREATE_POINT:
            if (controller->xkey_hold)
            {
                point_pool.emplace_back(std::make_shared<Point3D>(round(-((double)(origin.x - controller->GetMousePoint()->x)) / ((double)grid_size)), round(((double)(origin.y - controller->GetMousePoint()->y)) / ((double)grid_size)), 0.0));
            }
            else
            {
                point_pool.emplace_back(std::make_shared<Point3D>(-((double)(origin.x - controller->GetMousePoint()->x)) / ((double)grid_size), ((double)(origin.y - controller->GetMousePoint()->y)) / ((double)grid_size), 0.0));
            }
            break;
        case cmd_KEY::cmd_UNDO:
            if (!point_pool.empty())
            {
                point_pool.pop_back();
            }
            break;
        default:
            break;
        }
    }
}

PointNode::~PointNode()
{
    counter--;
}
