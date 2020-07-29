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


enum class DataType{Invalid, Data, Geometry, Point, Vector, Line, Polyline, Color};

class Node;

//need a resource manager to hold all the texture data, 
//for eachnode, you need 1. rectangle for render copy
//2. relative pointer pointing to the texture.
// >>>>> SDL_RenderCopy(renderer, texture, NULL, &dstrect);
struct IconManager
{
    IconManager(SDL_Renderer* renderer);
    ~IconManager();
    void LoadIcons(SDL_Renderer* renderer);
    void DrawIcon(SDL_Renderer* renderer, SDL_Rect* rect, std::string icon_name);
    SDL_Texture* QueryIcon(std::string icon_name);
    std::unordered_map<std::string, SDL_Texture*> map_Icons;
};



class NodeConnector
{
public:
    NodeConnector(Point2D<double> location, bool in_or_out, Node* parent, DataType type);
    ~NodeConnector();
    void Update(Telecontroller* controller, Point2D<double> location);
    Point2D<double> GetLocation(){return _pos;}

    //getters
    bool IsInput(){return _in_or_out;}
    bool IsPassing(){return _passing;}
    bool IsConnected(){return _connected;}
    bool IsSeleted(){return _selected;}
    bool IsConnectingMode(){return _connecting_mode;}
    void EstablishConnection(std::shared_ptr<NodeConnector> target);
    void Disconnect(){_input_target = nullptr; _connected = false;};
    void Draw(SDL_Renderer *renderer);
    void DrawConnection(SDL_Renderer *renderer, int mode);
    bool GetOnConnectDragMode(){return _connecting_mode;}
    void ToggleConnect(){_connected = !_connected;}

    DataType GetDataType(){return _type;}
    DataType GetTargetType(){return _input_target->GetDataType();}
    Node* GetTargetParent(){return _input_target->_parent;}
    NodeConnector* GetInputTargetAddress(){return _input_target.get();}

private:
    Node *_parent;
    std::shared_ptr<NodeConnector> _input_target;
    Point2D<double> _pos;
    SDL_Rect _rect;

    DataType _type;

    void UpdateLocation(Point2D<double> location);
    void DrawDot8x8(SDL_Renderer *renderer);
    bool _in_or_out; //is input or output

    bool _passing;
    bool _selected;
    bool _connecting_mode;
    bool _connected;
    //graphics
    static int _size;
    static SDL_Color _norcolor;
    static SDL_Color _passcolor;
    static SDL_Color _selcolor;
};




class Node
{
public:
    Node();
    virtual ~Node();

    //virtual functions
    virtual void Update(Telecontroller *controller, const Point2D<int> origin_s, double scale) = 0;
    virtual void DrawNode(SDL_Renderer *, std::shared_ptr<IconManager>)= 0;
    virtual void DrawGeometry(SDL_Renderer *, Point2D<int>& origin_s, int grid_size) const = 0;
    //this function only for editable node objects
    virtual void ProcessEditModeInput(Telecontroller *controller, const Point2D<int> origin, int grid_size) = 0;

    virtual void ProcessData() = 0;
    virtual std::vector<std::shared_ptr<GeoData>> GetOutputData() = 0;

    //setters and getters for all the node objects
    bool GetIfDrag(){return _ondrag;}
    bool GetIfPass(){return _passing;}
    bool GetIfConnecting(){return _dragconnect_mode;}
    bool GetIfGoingtoConnect(){return _attempt_got_connected;}
    bool GetIsSelected(){return _selected;}
    bool GetIsEditable(){return _editable;};
    bool GetIsDisplay(){return _displaying;}
    bool GetIsNodeExist(std::string name);
    Point2D<double> GetLocation(){return _center;}
    std::shared_ptr<NodeConnector> GetSelConnector();
    std::shared_ptr<NodeConnector> GetPassingConnector();

    bool HasInputLink();
    std::vector<std::string> GetInputParents();

    static size_t GetSelectionCount(){return selected_count;}
    static void SetSelectedCount(int count){selected_count = count;} 
    static void SetPassingCount(int count){passing_count = count;}
    static void ToggleGroupSelect(bool group_sel){if_groupsel = group_sel;}
    void SetAsSelected(){_selected = true;}
    void SetAsUnselected(){_selected = false;}
    void SetDisplay(){_displaying = true;}
    void SetNondisplay(){_displaying = false;}
    int GetWidth(){return _node_width;}
    int GetHeight(){return _node_height;}
    std::string GetName(){return _name;}
    

protected:
    bool _passing;
    bool _selected;
    bool _running;
    bool _ondrag;
    bool _dragconnect_mode;
    bool _attempt_got_connected;
    bool _editable;
    bool _displaying;
    bool _editing;

    static size_t node_counter;
    static size_t selected_count;
    static size_t passing_count;
    static bool if_groupsel;
    static bool is_editing_mode;
    //Graphics
    
    static int _node_width;
    static int _node_height;
    static int _icon_edge_len;

    static SDL_Color _node_color;
    static SDL_Color _selcolor;
    static SDL_Color _passcolor;
    static SDL_Color _textcolor;
    static SDL_Color _editmode_color;
    static SDL_Color _lockmode_color;
    static SDL_Color _renderflag_color;

    //info
    Point2D<double> _center;
    Point2D<double> _clicked_old_pos;
    SDL_Rect _icon_rect;
    SDL_Rect _node_rect;

    //connectors
    void UpdateConnectors(Telecontroller *controller, const Point2D<int> origin_s);
    void SetConnectorLocations(Telecontroller *controller, const Point2D<int> origin_s);
    void DrawNodeConnectors(SDL_Renderer* renderer);
    std::vector<std::shared_ptr<NodeConnector>> _inputs;
    std::vector<std::shared_ptr<NodeConnector>> _outputs;
    std::shared_ptr<NodeConnector> _current_sel_connector;
    std::shared_ptr<NodeConnector> _current_pass_connector;
    static int connection_line_style;
    
    std::string _name;
    static std::vector<std::string> _name_pool;
    void GetNonDuplicatedNames(std::string NODE_name);
    
    std::unique_ptr<ScreenText> _textdisplay;

    void ProcessUserInputs(Telecontroller *controller, const Point2D<int> origin_s, double scale);
    void ScreenTransform(const Point2D<int>& origin_s, double scale); 
    void DrawDisplayRects(SDL_Renderer* renderer, std::string icon_name, std::shared_ptr<IconManager> Icon_manager);

    //thread protect:
    static std::mutex _mutex;    
};






//Creating points, interact with Tall
class PointNode : public Node
{
public:
    PointNode(Point2D<double> drop_location, const Point2D<int>& origin_s, double scale);
    ~PointNode();

    void Update(Telecontroller *controller, const Point2D<int> origin_s, double scale) override;
    //this function only for editable node objects
    void ProcessEditModeInput(Telecontroller *controller, const Point2D<int> origin, int grid_size)override;
    void DrawNode(SDL_Renderer * renderer, std::shared_ptr<IconManager> Icm)override;
    void DrawGeometry(SDL_Renderer *renderer, Point2D<int>& origin_s, int grid_size) const override;
    void CreatePoints(double x, double y, double z);

    void ProcessData() override;
    std::vector<std::shared_ptr<GeoData>> GetOutputData() override;

private:
    //Data field
    std::vector<std::shared_ptr<Point3D>> point_pool;

    static size_t counter;
    bool pt_onlysel;
};



class MergeNode : public Node
{
public:

    MergeNode(Point2D<double> drop_location, const Point2D<int>& origin_s, double scale);
    ~MergeNode();

    void Update(Telecontroller *controller, const Point2D<int> origin_s, double scale)override;
    void DrawNode(SDL_Renderer *renderer, std::shared_ptr<IconManager> Icm)override;
    void DrawGeometry(SDL_Renderer *renderer, Point2D<int>& origin_s, int grid_size)const override;
    void ProcessEditModeInput(Telecontroller *controller, const Point2D<int> origin, int grid_size)override{};
    void ProcessData()override;
    std::vector<std::shared_ptr<GeoData>> GetOutputData()override;

    
private:
    static size_t counter;
    std::vector<std::shared_ptr<GeoData>> geo_pool;
};






class LineNode : public Node
{
public:
    LineNode(Point2D<double> drop_location, const Point2D<int>& origin_s, double scale);
    ~LineNode();
    void Update(Telecontroller *controller, const Point2D<int> origin_s, double scale)override;
    void DrawNode(SDL_Renderer *renderer, std::shared_ptr<IconManager> Icm)override;
    void DrawGeometry(SDL_Renderer *renderer, Point2D<int>& origin_s, int grid_size)const override;
    void ProcessEditModeInput(Telecontroller *controller, const Point2D<int> origin, int grid_size)override{};
    void ProcessData()override;
    std::vector<std::shared_ptr<GeoData>> GetOutputData()override;
private:
    std::vector<std::shared_ptr<Line>> line_pool;
    static size_t counter;
};






class PolylineNode : public Node
{
public:
    PolylineNode(Point2D<double> drop_location, const Point2D<int>& origin_s, double scale);
    ~PolylineNode();
    void Update(Telecontroller *controller, const Point2D<int> origin_s, double scale) override;
    void DrawNode(SDL_Renderer *renderer, std::shared_ptr<IconManager> Icm) override;
    void DrawGeometry(SDL_Renderer *renderer, Point2D<int> &origin_s, int grid_size) const override;
    void ProcessEditModeInput(Telecontroller *controller, const Point2D<int> origin, int grid_size) override{};
    void ProcessData() override;
    std::vector<std::shared_ptr<GeoData>> GetOutputData() override;

private:
    std::vector<std::shared_ptr<Curve>> geo_pool;
    static size_t counter;
};





class CurveNode : public Node
{
public:
    CurveNode(Point2D<double> drop_location, const Point2D<int>& origin_s, double scale);
    ~CurveNode();
    void Update(Telecontroller *controller, const Point2D<int> origin_s, double scale)override;
    void DrawNode(SDL_Renderer *renderer, std::shared_ptr<IconManager> Icm)override;
    void DrawGeometry(SDL_Renderer *renderer, Point2D<int>& origin_s, int grid_size)const override;
    void ProcessEditModeInput(Telecontroller *controller, const Point2D<int> origin, int grid_size)override{};
    void ProcessData()override;
    std::vector<std::shared_ptr<GeoData>> GetOutputData()override;
private:
    std::vector<std::shared_ptr<Curve>> geo_pool;
    static size_t counter;
};

#endif