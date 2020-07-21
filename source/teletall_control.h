#ifndef TELETALL_CONTROL_H
#define TELETALL_CONTROL_H
#include <string>
#include <vector>
#include "JUTA/JUTA_geometry_core.h"
#include "teletall_console.h"


enum class PanelID {ON_PAD,ON_BAR,ON_MENU,ON_TALL,ON_CONSOLE, NONE};
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
    void CloseAllMenusTrue(){_closeAllMenus = true;};
    bool ShouldCloseMenu(){return _closeAllMenus;}
    cmd_KEY GetCommand(){return cmd;}
    std::string GetMsg(){return _Msg;}

    void LinkPadRect(SDL_Rect* in_PadRect, SDL_Rect* in_MSlidRec);
    void LinkTallRec(SDL_Rect* in_TallRect);
    void LinkMenuRec(SDL_Rect* in_Menu);
    SDL_Rect* GetPadRect(){ return &r_PadRect;}
    SDL_Rect* GetTallRect(){return &r_TallRect;}
    void SetCurrentPanel(PanelID panel){current_panel = panel;}
    PanelID GetCurrentPanel(){return current_panel;}

    bool MouseL_hold;
    bool MouseR_hold;
    bool MouseM_hold;
    Uint8 clicktime;

private:
    Uint8 clickstamp;
    int _split_location; 
    std::vector<std::string> _msg;
    SDL_Window** _hhwnd;
    Point2D<int> _mouseLocation;
    Console* console;
    cmd_KEY cmd;
    bool key_ctrl;
    bool key_alt;
    bool key_shift;
    bool _closeAllMenus;
    std::string _Msg;
    SDL_Rect r_PadRect;
    SDL_Rect r_TallRect;
    SDL_Rect r_MSliderRect;
    SDL_Rect r_MenuRect;
    SDL_Rect r_ConsoleRect;
};

#endif