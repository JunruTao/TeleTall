#ifndef TELENODE_H
#define TELENODE_H

#include "x_sdl2headers.h"
#include "x_threadheaders.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <algorithm>
#include "ui/teletall_graphics.h"
#include "JUTA/JUTA_geometry_core.h"
#include "JUTA/JUTA_math.h"
#include "teletall_control.h"


struct IconManager
{
    IconManager(SDL_Renderer* renderer);
    ~IconManager();
    void LoadIcons(SDL_Renderer* renderer);
    void DrawIcon(SDL_Renderer* renderer, SDL_Rect* rect, std::string icon_name);
    SDL_Texture* QueryIcon(std::string icon_name);
    std::unordered_map<std::string, SDL_Texture*> map_Icons;
};






class Node
{
public:

    virtual void Update(Telecontroller *controller, const Point2D<int> origin_s, double scale) = 0;
    virtual void DrawNode(SDL_Renderer *, std::shared_ptr<IconManager>)= 0;
    virtual void DrawGeometry(SDL_Renderer *) const = 0;
    virtual void RecieveData() = 0;
    virtual void ProcessData() = 0;
    virtual void SendData() = 0;
    Point2D<double> GetLocation(){return _center;}
    static size_t GetSelectionCount(){return selected_count;}
    static int GetOnDragMotion(){return ondrag_any;}
    void SetAsSelected(){_selected = true; selected_count ++; }
    void SetAsUnselected(){_selected = false; selected_count --;}

protected:
    bool _passing;
    bool _selected;
    bool _running;
    bool _ondrag;
    static size_t selected_count;
    static int ondrag_any;
    //Graphics
    
    static int _node_width;
    static int _node_height;
    static int _icon_edge_len;
    static SDL_Color _node_color;
    static SDL_Color _selcolor;
    static SDL_Color _passcolor;
    static SDL_Color _textcolor;

    //info
    Point2D<double> _center;
    Point2D<double> _clicked_old_pos;
    SDL_Rect _icon_rect;
    SDL_Rect _node_rect;
    
    std::string _name;
    std::unique_ptr<ScreenText> _textdisplay;

   
};

//need a resource manager to hold all the texture data, 
//for eachnode, you need 1. rectangle for render copy
//2. relative pointer pointing to the texture.
// >>>>> SDL_RenderCopy(renderer, texture, NULL, &dstrect);




class NodeConnector
{
public:
    NodeConnector(Point2D<double> location, bool in_or_out): _pos(location), _in_or_out(in_or_out), _selected(false){};
    void EstablishConnection(NodeConnector *target);
    void Draw(SDL_Renderer *renderer);
    void DrawConnection(SDL_Renderer *renderer);

private:
    Node *_parent;
    Node *_target;
    Point2D<double> _pos;

    bool _in_or_out; //is input or output
    bool _selected;
    //graphics
    static SDL_Color _norcolor;
    static SDL_Color _passcolor;
    static SDL_Color _selcolor;
};




//Creating points, interact with Tall
class PointNode : public Node
{
public:
    PointNode(Point2D<double> drop_location, const Point2D<int>& origin_s, double scale);
    ~PointNode();

    void Update(Telecontroller *controller, const Point2D<int> origin_s, double scale) override;
    
    void DrawNode(SDL_Renderer * renderer, std::shared_ptr<IconManager> Icm)override;
    void DrawGeometry(SDL_Renderer *) const override { };

    void RecieveData() override { };
    void ProcessData() override { };
    void SendData() override { };

private:
    //meta data
    static std::string _icon_name;
    

    //status
    bool _manipulatable;


    //information
    std::vector<NodeConnector*> _inputs;
    std::vector<NodeConnector*> _outputs;
    static size_t counter;

    //graphics
    void ScreenTransform(const Point2D<int>& origin_s, double scale);
};










class LineNode : public Node
{

};

class PolylineNode : public Node
{

};

class NurbsCurveNode : public Node
{

};

#endif