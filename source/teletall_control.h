#ifndef TELETALL_CONTROL_H
#define TELETALL_CONTROL_H
#include <SDL.h>
#include <string>
#include <vector>
#include "JUTA/JUTA_geometry_core.h"

enum class PanelID {ON_PAD,ON_BAR,ON_MENU,ON_TALL};

class Telecontroller
{
public:
    Telecontroller();
    ~Telecontroller();

    void ProcessInput(bool& running);
    void SendToConcole();
    void StoreHWND(SDL_Window** hhwnd){_hhwnd = hhwnd;}
    Point2D<int>* GetMousePoint();
    SDL_Window* GetHWND()const{return *_hhwnd;};
    
    //for these 2 panels
    int GetSplitLocation()const;
    void UpdateSplitLocation(int new_split);

    static bool RMB_hold;
    static bool LMB_hold;
    static bool MMB_hold;

    bool key_HOME;
    bool key_FRAME;
    PanelID current_panel;

    bool eWinUpdate;
    
    
private:
    int _split_location; 
    std::vector<std::string> _msg;
    SDL_Window** _hhwnd;
    Point2D<int> _mouseLocation;

};

#endif