#ifndef TELEPAD_H
#define TELEPAD_H

#include "x_sdl2headers.h"
#include <vector>
#include "teletall_control.h"
#include "JUTA/JUTA_math.h"
#include "JUTA/JUTA_geometry_core.h"
#include "ui/teletall_graphics.h"
#include "ui/teletall_button.h"
#include "node/telenode.h"
#include "tallwindow.h"

class Telepad
{
public:
    Telepad(const size_t &master_width, const size_t &master_height);
    ~Telepad();

    void Update(Telecontroller &controller);
    void Render(SDL_Renderer *renderer);
    void AssignIconManager(std::shared_ptr<IconManager>& Icm);
    void StartNodeProcessThread();
    void LinkTall(Tallwindow* htall){_tall = htall;}
    Point2D<int>* GetOrigin();

private:
    size_t win_width;
    size_t win_height;
    size_t pad_width;
    size_t pad_height;
    SDL_Rect padViewport;
    SDL_Rect slidebar;
    bool slidebarSelected;
    bool gridSelected;
    bool onPad;
    bool onBar;
    bool onTall;
    
    std::vector<Point2D<int>> mouse_trail;
    
    //The grid
    Point2D<int> origin;
    int grid_size;
    SDL_Color gridcolor;
    void DrawSlideBar(SDL_Renderer *renderer);
    
    void MoveGrid(Telecontroller &controller, const int &x, const int &y);
    void Resize(Telecontroller &controller, const int &x, const int &y);

    //nodes
    std::vector<std::shared_ptr<Node>> node_pool;

    //Send render lists to tall:
    void SendNodesToTall(std::shared_ptr<Node>& node_to_tall);

    void CreateNode(Telecontroller& controller, int x, int y);
    void DeleteNode();
    void UpdateNode(Telecontroller* controller);
    void Simulate(){};
    void DrawNodes(SDL_Renderer* renderer);

    //Selection;
    void Select(Telecontroller* controller, int x, int y);
    void DrawSelectionRect(SDL_Renderer* renderer);
    void DrawConnectCurve(SDL_Renderer* renderer);
    std::shared_ptr<NodeConnector> _sel_connector;
    std::vector<std::shared_ptr<Node>> _sel_nodes;
    bool _selecting;
    bool _connecting;

    //ResourceManagers
    std::shared_ptr<IconManager> _Icm;

    //geometry renderer
    Tallwindow* _tall;

    //working thread
    std::thread* worker_thread;
    void ProcessNodesIO();
    bool stop_thread;

};

#endif