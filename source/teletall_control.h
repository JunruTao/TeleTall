#ifndef TELETALL_CONTROL_H
#define TELETALL_CONTROL_H
#include <string>
#include <vector>
#include "JUTA/JUTA_geometry_core.h"
#include "teletall_console.h"


enum class PanelID {ON_PAD,ON_BAR,ON_MENU,ON_TALL,ON_CONSOLE};
enum class MouseLockID {FREE, TALL_LOCKED, TELE_LOCKED};


class Telecontroller
{
public:
    Telecontroller();
    ~Telecontroller();

    void ProcessInput(bool& running);
    void SendToConcole();
    void DrawSelectionRect(SDL_Renderer* renderer);
    
    void StoreHWND(SDL_Window** hhwnd){_hhwnd = hhwnd;}
    Point2D<int>* GetMousePoint();
    SDL_Window* GetHWND()const{return *_hhwnd;};
    
    //for these 2 panels
    int GetSplitLocation()const;
    void UpdateSplitLocation(int new_split);

    MouseLockID Shared_Nevigation_Lock;
    PanelID current_panel;

    void SendCommand(const cmd_KEY& _cmd) {cmd = _cmd;};
    void SendCommandEx(const cmd_KEY& _cmd, std::string _msg);
    cmd_KEY GetCommand(){return cmd;}

    void LinkPadRect(SDL_Rect* in_PadRect, SDL_Rect* in_MSlidRec);
    void LinkTallRec(SDL_Rect* in_TallRect);

    bool MouseL_hold;
    bool MouseR_hold;
    bool MouseM_hold;

private:
    int _split_location; 
    std::vector<std::string> _msg;
    SDL_Window** _hhwnd;
    Point2D<int> _mouseLocation;
    Console* console;
    cmd_KEY cmd;
    bool key_ctrl;
    bool key_alt;
    bool key_shift;
    std::string _Msg;
    SDL_Rect r_PadRect;
    SDL_Rect r_TallRect;
    SDL_Rect r_MSliderRect;
};

#endif