#include "x_threadheaders.h"
#include "teletall.h"
#include "telepad.h"
#include "tallwindow.h"
#include "teletall_control.h"
#include "ui/teletall_graphics.h"
#include <iostream>
#include <chrono>


#define SCREEN_TEXTSIZE 15

int main(int argc, char *argv[])
{
    /*BASIC PROPERTIES*/
    //Width and Height of the window
    constexpr size_t tltl_Window_Width{900};
    constexpr size_t tltl_Window_Height{600};
    //Frame rate this program supposed to run at
    constexpr size_t tltl_Frame_Rate{60};
    
    //creating the app;
    TeleTall tltlApp(tltl_Window_Width, tltl_Window_Height);
    //{required}Loading Textfont data here.
    ScreenText::InitLoadMedia(SCREEN_TEXTSIZE);

    /*CREATE THE PROGRAM CORE*/
    Telecontroller controller;
    Telepad myTelePad(tltl_Window_Width, tltl_Window_Height);
    Tallwindow myTallwindow(tltl_Window_Width, tltl_Window_Height);

    

    /*RUN THE PROGRAM*/
    tltlApp.Run(tltl_Frame_Rate, myTelePad, myTallwindow, controller);

    return 0;
}
