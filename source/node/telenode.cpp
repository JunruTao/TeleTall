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





//* Node class -static members
int Node::_node_width = 100;
int Node::_node_height = 30;
int Node::_icon_edge_len = 24;
size_t Node::selected_count = 0;
size_t Node::passing_count = 0;
bool Node::if_groupsel = false;
SDL_Color Node::_node_color = {100, 100, 100, 255};
SDL_Color Node::_selcolor = {35,240,127,255}; //outline
SDL_Color Node::_passcolor = {200,200,200,255}; //outline
SDL_Color Node::_textcolor = {120,120,120,255};

void Node::ProcessUserInputs(Telecontroller *controller, const Point2D<int> origin_s, double scale)
{
    
    bool inboud = controller->GetMousePoint()->InBoundWH(_node_rect.x, _node_rect.y, _node_rect.w, _node_rect.h);

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
            selected_count++; //here a node got selected
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

        }else
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
        
        if(controller->GetCommand() == cmd_KEY::cmd_LMB && (controller->GetCurrentPanel() == PanelID::ON_PAD))
        {   
            _selected = false;
            selected_count = 0;
            if_groupsel = false;
        }

     
    }

    skip:{}

    _clicked_old_pos.x = (controller->GetMousePoint()->x - origin_s.x);
    _clicked_old_pos.y = (controller->GetMousePoint()->y - origin_s.y);
}

/*

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
        (relative) Pn = << Cen.x - (NodeHeight / 2), Cen.y - (NodeWidth / 2) >>
        (screen) Pn = << (Cen.x + O.x) - (NodeHeight / 2), (Cen.y + O.y) - (NodeWidth / 2)  >>

*/


//node connector
SDL_Color NodeConnector::_norcolor = {200,200,200,255}; //dot color - normal
SDL_Color NodeConnector::_selcolor = {35,240,127,255}; //dot color - selected
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
        if(controller->MouseL_hold)
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

/*===================================================================================================

//field info
//status

    // bool _manipulatable;
    // bool _selected;
    // bool _running;

//information
    // Point2D<double> _center;
    // SDL_Rect _icon_rect;
    // std::string _name;
    // std::vector<NodeConnector*> _inputs;
    // std::vector<NodeConnector*> _outputs;

//Point Node class

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


std::string PointNode::_icon_name = "pointnode";
size_t PointNode::counter = 0;
//constructor:
PointNode::PointNode(Point2D<double> drop_location, const Point2D<int>& origin_s, double scale)
{
    counter++;

    _manipulatable = false;
    _selected = false;
    _passing = false;
    _running = false;
    _ondrag = false;

    _name = "Point" + std::to_string(counter);
    _center = drop_location;
    _clicked_old_pos = _center;

    Point2D<double> input_cloc(_center.x + origin_s.x, _center.y + (_node_height/2) + origin_s.y);
    Point2D<double> output_cloc(_center.x + origin_s.x, _center.y - (_node_height/2) + origin_s.y);
    
    //Creating input buttons
    _inputs.emplace_back(new NodeConnector(input_cloc,true, this));
    _outputs.emplace_back(new NodeConnector(output_cloc,false, this));

    ScreenTransform(origin_s,scale);
    _textdisplay = std::make_unique<ScreenText>();
    
}




void PointNode::ScreenTransform(const Point2D<int>& origin_s, double scale)
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
        }
    }
    for(auto& n : _outputs)
    {  
        if(n->GetOnConnectDragMode())
        {
            connecting++;
        }
    }
    
    if(connecting>0)
    {
        _dragconnect_mode = true;
    }else
    {
        _dragconnect_mode = false;
    }
}




void PointNode::DrawNode(SDL_Renderer * renderer, std::shared_ptr<IconManager> Icm)
{
    SDL_SetRenderDrawColor(renderer,_node_color.r,_node_color.g,_node_color.b,_node_color.a);
    SDL_RenderFillRect(renderer,&_node_rect);
    SDL_SetRenderDrawColor(renderer,_node_color.r + 20 ,_node_color.g + 20 ,_node_color.b + 20,_node_color.a);
    SDL_RenderDrawRect(renderer,&_node_rect);


    SDL_SetRenderDrawColor(renderer,_node_color.r - 12 ,_node_color.g - 12 ,_node_color.b - 12,_node_color.a);
    SDL_RenderDrawLine(renderer, _node_rect.x+1,_node_rect.y+1,_node_rect.x+_node_rect.w-2,_node_rect.y+1);
    SDL_RenderDrawLine(renderer, _node_rect.x+1, _node_rect.y+_node_rect.h -2,_node_rect.x+_node_rect.w-2,_node_rect.y+_node_rect.h -2);



    Icm->DrawIcon(renderer,&_icon_rect,_icon_name);
    _textdisplay->loadFromRenderedText(_name, renderer, _textcolor, _node_color, 3);
    _textdisplay->Draw(renderer,_node_rect.x + _node_width+3, _node_rect.y + (_node_rect.h/2) - (ScreenText::GetUniversalTextHeight()/2));

    if(_passing && !_selected)
    {
        SDL_SetRenderDrawColor(renderer,_passcolor.r,_passcolor.g ,_passcolor.b,_passcolor.a);
        SDL_RenderDrawRect(renderer,&_node_rect);
    }else if(_selected)
    {
        SDL_SetRenderDrawColor(renderer,_selcolor.r,_selcolor.g ,_selcolor.b,_selcolor.a);
        SDL_RenderDrawRect(renderer,&_node_rect);
    }else if(_dragconnect_mode)
    {
        SDL_SetRenderDrawColor(renderer,0,120,255,_selcolor.a);
        SDL_RenderDrawRect(renderer,&_node_rect);
    }

    for(auto& n:_inputs)
    {
        n->Draw(renderer);
    }
    for(auto& n:_outputs)
    {
        n->Draw(renderer);
    }
}




PointNode::~PointNode()
{
    if (!_inputs.empty())
    {
        for (auto &e : _inputs)
        {
            delete e;
        }
    }

    if (!_outputs.empty())
    {
        for (auto &e : _outputs)
        {
            delete e;
        }
    }
    counter--;
}
