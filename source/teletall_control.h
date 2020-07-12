#ifndef TELETALL_CONTROL_H
#define TELETALL_CONTROL_H
#include <SDL2/SDL.h>


class Telecontroller
{
public:
    Telecontroller();
    ~Telecontroller();

    void ProcessInput(bool& running);
    void Synchoronise();
    
    int GetSplitLocation()const;

    static int lastMouseLocation_X;
    static int lastMouseLocation_Y;
    static int nowMouseLocation_X;
    static int nowMouseLocation_Y;
    static bool RMB_hold;
    static bool LMB_hold;
    static bool MMB_hold;

    bool key_HOME;
    bool key_FRAME;

};

#endif