#include "teletall.h"

int main(int argc, char *argv[])
{ 
    /*BASIC PROPERTIES*/ 
        //Width and Height of the window
    constexpr size_t tltl_Window_Width {900};
    constexpr size_t tltl_Window_Height{600};
        //Frame rate this program supposed to run at
    constexpr size_t tltl_Frame_Rate{60};

    /*CREATE THE PROGRAM CORE*/ 
    TeleTall tltlApp(tltl_Window_Width, tltl_Window_Height);\
    /*RUN THE PROGRAM*/
    tltlApp.Run(tltl_Frame_Rate);

    return 0;
}
