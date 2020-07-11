#ifndef TELETALL_H
#define TELETALL_H
//include area
#include <SDL2/SDL.h>

class TeleTall
{
    public:
    //Constructor: (Initialise all context)
    TeleTall(size_t tltl_Window_Width, size_t tltl_Window_Height);
    //Destructor: (Free all resource)
    ~TeleTall();

    //Running Program: while loop here
    void Run(size_t tltl_Frame_Rate);
    void Update();

    private:
    void ErrorReporter(const char* errorMessage);

    SDL_Window* hwnd_main;
    SDL_Renderer *hRenderer;


};

#endif