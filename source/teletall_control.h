#ifndef TELETALL_CONTROL_H
#define TELETALL_CONTROL_H
#include <SDL2/SDL.h>


class Telecontroller
{
public:
    Telecontroller();
    ~Telecontroller();

    void ProcessInput(bool& running);
    
    int GetSplitLocation()const;

    static size_t lastMouseLocation_X;
    static size_t lastMouseLocation_Y;
    static size_t nowMouseLocation_X;
    static size_t nowMouseLocation_Y;
    static bool RMB_hold;
    static bool LMB_hold;
    static bool MMB_hold;
    //static bool MOUSE_move;
    int _tltlvertprop;
private:
    
    
};

#endif