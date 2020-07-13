#include "teletall.h"


#include <string>

#define WINDOW_NAME "TeleTall - SDL2 Version - v.01"

//[CONSTRUCTOR]-------------------------------------------------------------------------------------
TeleTall::TeleTall(const size_t &tltl_Window_Width, const size_t &tltl_Window_Height)
{
    //[1] SDL initialisation:
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        ErrorReporter("SDL initialisation failed."); //Test Success;

    //[2] Creating SDL_Window:
    hwnd_main = SDL_CreateWindow(
        WINDOW_NAME,            //--Window title
        SDL_WINDOWPOS_CENTERED, //--Place window in the center of the screen
        SDL_WINDOWPOS_CENTERED, //--Place window in the center of the screen
        tltl_Window_Width,      //--Main Window Width
        tltl_Window_Height,     //--Main Window Height
        SDL_WINDOW_RESIZABLE);      //--Show window command SDL
    if (hwnd_main == NULL)
        ErrorReporter("SDL window creation failed. "); //Test Success;

    //[3] Create the renderer:
    hRenderer = SDL_CreateRenderer(hwnd_main, -1, SDL_RENDERER_ACCELERATED);
    if (hRenderer == NULL)
        ErrorReporter("SDL Renderer creation failed. "); //Test Success;
}

//[DESTRUCTOR]--------------
TeleTall::~TeleTall()
{
    SDL_DestroyWindow(hwnd_main);
    SDL_Quit();
}

//[RUN]---------------------
void TeleTall::Run(
    size_t tltl_Frame_Rate,     //Frame rate designated ms per loop
    Telepad &telepad,           //Telepad
    Tallwindow &tallwindow,     //Tallwindow
    Telecontroller &controller) //Telecontroller
{
    controller.StoreHWND(&hwnd_main);

    //Field For time measuring
    Uint32 time_stamp = SDL_GetTicks();
    Uint32 frame_start;
    Uint32 frame_end;
    Uint32 frame_duration;
    unsigned int frame_count = 0;
    unsigned int MsPerFrame = 1000 / tltl_Frame_Rate;

    //Field for toggles
    bool running = true;
    bool tltl_showframerate = true;

    /*The Feedback Loop*/
    while (running)
    {
        frame_start = SDL_GetTicks();
        //handle inputs and events here:
        //..
        controller.ProcessInput(running);

        //Update Function here:
        //..
        telepad.Update(controller);
        tallwindow.Update(controller);

        //Render here:
        //..


        tallwindow.Render(hRenderer);
        telepad.Render(hRenderer);

        SDL_RenderPresent(hRenderer);
        //.
        //.

        //------------------------TIME---------------------------
        //Time Frame Section(calculate current framerate and stablise)
        frame_end = SDL_GetTicks();
        frame_count++;
        frame_duration = frame_end - frame_start;
        if (frame_end - time_stamp >= 1000)
        {
            //...set tile every second
            if (true)
            {
                std::string title{
                    std::string(WINDOW_NAME) 
                    + "   -running status: " 
                    + std::to_string(frame_count) 
                    + " fps"};

                SDL_SetWindowTitle(hwnd_main, title.c_str());
            }
            //clearing the count
            frame_count = 0;
            time_stamp = frame_end;
        }

        if (frame_duration < MsPerFrame)
        {
            //using this to stablise the machine
            SDL_Delay(MsPerFrame - frame_duration);
        }

        //------------------------TIME---------------------------
    }
}

//[Utility] for reporting errors------------------------------------------------------------
void TeleTall::ErrorReporter(const char *errorMessage)
{
    SDL_ShowSimpleMessageBox(0, "TeleTall-SDL: Error", errorMessage, NULL);
}
