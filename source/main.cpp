#include "teletall.h"
#include "telepad.h"
#include "tallwindow.h"
#include "teletall_control.h"


int main(int argc, char *argv[])
{
    /*BASIC PROPERTIES*/
    //Width and Height of the window
    constexpr size_t tltl_Window_Width{900};
    constexpr size_t tltl_Window_Height{600};
    //Frame rate this program supposed to run at
    constexpr size_t tltl_Frame_Rate{30};

    /*CREATE THE PROGRAM CORE*/
    Telecontroller controller;
    Telepad myTelePad(tltl_Window_Width, tltl_Window_Height, controller);
    Tallwindow myTallwindow(tltl_Window_Width, tltl_Window_Height, controller);

    TeleTall tltlApp(tltl_Window_Width, tltl_Window_Height);
    /*RUN THE PROGRAM*/
    tltlApp.Run(tltl_Frame_Rate, myTelePad, myTallwindow, controller);

    return 0;
}
