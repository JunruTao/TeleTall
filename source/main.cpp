/////////////////////////////TELETALL 2020///////////////////////////
////IF Building on Linux, please change this to _WINUSER_ to 0///////
/////////////////////////////////////////////////////////////////////
#define _WINUSER_ 1

#if _WINUSER_ == 1
#define _WIN32_WINNT  0x0601 
#endif

#if _WINUSER_ == 1
#include "mingw.thread.h"
#endif
#if _WINUSER_ == 0
#include <thread>
#endif

#include "teletall.h"
#include "telepad.h"
#include "tallwindow.h"
#include "teletall_control.h"
#include "ui/teletall_graphics.h"
#include <iostream>
#include <chrono>


int main(int argc, char *argv[])
{
    /*BASIC PROPERTIES*/
    //Width and Height of the window
    constexpr size_t tltl_Window_Width{900};
    constexpr size_t tltl_Window_Height{600};
    //Frame rate this program supposed to run at
    constexpr size_t tltl_Frame_Rate{60};
    
    /*CREATE THE PROGRAM CORE*/
    Telecontroller controller;
    Telepad myTelePad(tltl_Window_Width, tltl_Window_Height);
    Tallwindow myTallwindow(tltl_Window_Width, tltl_Window_Height);

    TeleTall tltlApp(tltl_Window_Width, tltl_Window_Height);

    //{required}Loading Textfont data here.
    ScreenText::InitLoadMedia(15);


    /*RUN THE PROGRAM*/
    tltlApp.Run(tltl_Frame_Rate, myTelePad, myTallwindow, controller);

    return 0;
}
