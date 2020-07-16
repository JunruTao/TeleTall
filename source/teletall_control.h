#ifndef TELETALL_CONTROL_H
#define TELETALL_CONTROL_H
#include "x_sdl2headers.h"
#include <string>
#include <vector>
#include "JUTA/JUTA_geometry_core.h"

enum class PanelID {ON_PAD,ON_BAR,ON_MENU,ON_TALL};
enum class MouseLockID{FREE, TALL_LOCKED, TELE_LOCKED};

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

    static bool RMB_hold;
    static bool LMB_hold;
    static bool MMB_hold;

    bool key_HOME;
    bool key_FRAME;
    
    MouseLockID Shared_Nevigation_Lock;
    PanelID current_panel;

    bool eWinUpdate;
    

    void LinkPadRect(SDL_Rect* in_PadRect, SDL_Rect* in_MSlidRec)
    {
        r_PadRect.x = in_PadRect->x;
        r_PadRect.y = in_PadRect->y;
        r_PadRect.w = in_PadRect->w;
        r_PadRect.h = in_PadRect->h;
        r_MSliderRect.x = in_MSlidRec->x;
        r_MSliderRect.y = in_MSlidRec->y;
        r_MSliderRect.w = in_MSlidRec->w;
        r_MSliderRect.h = in_MSlidRec->h;
    }

    void LinkTallRec(SDL_Rect* in_TallRect)
    {
        r_TallRect.x = in_TallRect->x;
        r_TallRect.y = in_TallRect->y;
        r_TallRect.w = in_TallRect->w;
        r_TallRect.h = in_TallRect->h;
    }

private:
    int _split_location; 
    std::vector<std::string> _msg;
    SDL_Window** _hhwnd;
    Point2D<int> _mouseLocation;

    SDL_Rect r_PadRect;
    SDL_Rect r_TallRect;
    SDL_Rect r_MSliderRect;
};

#endif