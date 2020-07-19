#ifndef TELENODE_H
#define TELENODE_H

#include "x_sdl2headers.h"
#include <string>
#include "ui/teletall_graphics.h"
#include "JUTA/JUTA_geometry_core.h"
#include "JUTA/JUTA_math.h"
#include "teletall_control.h"


class Node
{
public:

    virtual void CreateNode() = 0;
    virtual void DeleteNode() = 0;
    virtual void Update(Telecontroller *controller) = 0;
    virtual void DrawNode(SDL_Renderer *) const = 0;
    virtual void DrawGeometry(SDL_Renderer *) const = 0;
    virtual void RecieveData() = 0;
    virtual void ProcessData() = 0;
    virtual void SendData() = 0;

protected:
    //Graphics
    Point2D<double> _nodecenter;
    int _node_width;
    int _node_height;
    SDL_Rect _node_rect;
    SDL_Color _node_color;
    SDL_Color _selcolor;
    SDL_Color _passcolor;
    bool _selected;
    bool _running;
    //information
    std::string _name;
    std::vector<NodeConnector> _inputs;
    std::vector<NodeConnector> _outputs;
   
};

class NodeConnector
{
public:
    void EstablishConnection(NodeConnector *target);
    void Draw(SDL_Renderer *renderer);
    void DrawConnection(SDL_Renderer *renderer);

private:
    Node *_parent;
    Point2D<double> _pos;
    bool _in_or_out; //is input or output
    bool _selected;
    SDL_Color _norcolor;
    SDL_Color _passcolor;
    SDL_Color _selcolor;
};

//Creating points, interact with Tall
class PointNode : public Node
{

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