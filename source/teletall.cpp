#include "teletall.h"
#include "ui/teletall_graphics.h"
#include "ui/teletall_button.h"
#include <string>
#include "node/telenode.h"

#define VERSION "v0.2.1"
#define WINDOW_NAME "TeleTall - Beta - "

//[CONSTRUCTOR]-------------------------------------------------------------------------------------
TeleTall::TeleTall(const size_t &tltl_Window_Width, const size_t &tltl_Window_Height)
{
    //[1.1] SDL initiation:
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        ErrorReporter("SDL initialisation failed."); //Test Success;
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        ErrorReporter("SDL Warning: Linear texture filtering not enabled!"); //Test Success;

    //[1.2] SDL_ttf initiation
    if(TTF_Init()==-1) 
        ErrorReporter("SDL_ttf initialisation failed");
        
    //[1.3] SDL_image initiation
    int flags = IMG_INIT_JPG | IMG_INIT_PNG;// load support for the JPG and PNG image formats
    int initted = IMG_Init(flags);
    if ((initted & flags) != flags)
        ErrorReporter("SDL_image initialisation failed");


    //[2] Creating SDL_Window:
    win_title = {std::string(WINDOW_NAME) + std::string(VERSION)};
    hwnd_main = SDL_CreateWindow(
        win_title.c_str(),            //--Window title
        SDL_WINDOWPOS_CENTERED, //--Place window in the center of the screen
        SDL_WINDOWPOS_CENTERED, //--Place window in the center of the screen
        tltl_Window_Width,      //--Main Window Width
        tltl_Window_Height,     //--Main Window Height
        SDL_WINDOW_RESIZABLE);      //--Show window command SDL
    if (hwnd_main == NULL)
        ErrorReporter("SDL window creation failed. "); //Test Success;

    //[3] Create the renderer:
    hRenderer = SDL_CreateRenderer(hwnd_main, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (hRenderer == NULL)
        ErrorReporter("SDL Renderer creation failed. "); //Test Success;

    
}






//[DESTRUCTOR]--------------
TeleTall::~TeleTall()
{
    if (topmenu != nullptr)
    {
        delete topmenu;
    }
    SDL_DestroyWindow(hwnd_main);
    TTF_Quit();
    IMG_Quit();
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
    controller.SendCommandEx(cmd_KEY::cmd_STARTUP, " * version number " + std::string(VERSION));
    controller.SendCommandEx(cmd_KEY::cmd_STARTUP, "program running...");

    //Field For time measuring
    Uint32 time_stamp = SDL_GetTicks();
    Uint32 frame_start;
    Uint32 frame_end;
    Uint32 frame_duration;
    unsigned int frame_count = 0;
    unsigned int MsPerFrame = 1000 / tltl_Frame_Rate;



    //Create the resource managers here:
    std::shared_ptr<IconManager> Icm = std::make_shared<IconManager>(hRenderer);
    //pass the Icon manager to telepad
    telepad.AssignIconManager(Icm);


    //Field for toggles
    bool running = true;
    bool tltl_showframerate = true;

    
    ConstructMenu();
    controller.SetCurrentPanel(PanelID::ON_PAD);
    controller.SendCommand(cmd_KEY::cmd_EMPTY);
    
    SDL_Event event;
    SDL_PollEvent(&event);
    /*The Feedback Loop*/
    while (running)
    {
        
        frame_start = SDL_GetTicks();
        //handle inputs and events here:
        //..
        controller.ProcessInput(running);

        //Update Function here:
        //..
        topmenu->Update(&controller);
        telepad.Update(controller);
        tallwindow.Update(controller);
        
        //controller.ProcessInput(running);

        //Render here:
        //..


        tallwindow.Render(hRenderer);
        telepad.Render(hRenderer);
        controller.DrawSelectionRect(hRenderer);

        topmenu->Draw(hRenderer);


        //Render before this line
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
                    win_title 
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
    }//end while



}

//[Utility] for reporting errors------------------------------------------------------------
void TeleTall::ErrorReporter(const char *errorMessage)
{
    SDL_ShowSimpleMessageBox(0, "TeleTall-SDL: Error", errorMessage, NULL);
}



//[Construct Menu]:
void TeleTall::ConstructMenu()
{
    typedef std::vector<std::string> NameList;
    typedef std::vector<cmd_KEY> __KeyList__;

    //Menu Section:
   NameList titles = {
        "File",
        "Edit",
        "View",
        "Nodes",
        "About"};

    topmenu = new BarMenu(hwnd_main, titles, 80);



    __KeyList__ title_cmds = 
    {
        cmd_KEY::cmd_EMPTY,
        cmd_KEY::cmd_EMPTY,
        cmd_KEY::cmd_EMPTY,
        cmd_KEY::cmd_EMPTY,
        cmd_KEY::cmd_WIN_ABOUT
    };
    topmenu->AddCommand(title_cmds);

    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    NameList title2 = {
        "Open                              Ctrl+O",
        "Save                              Ctrl+S",
        "Save As                Shift+Ctrl+S",
        "Options",
        "Quit"};
    ColumnMenu *subm_file = new ColumnMenu(title2, 200);

    NameList title_bella = {
        "I",
        "Love you",
        "Very Much",
    };
    ColumnMenu *subm_Bella = new ColumnMenu(title_bella, 200);
    ColumnMenu *subm_Bella2 = new ColumnMenu(title_bella, 200);

    NameList title_option = {
        "Option 1",
        "Option 2",
        "Bella",
    };
    
    ColumnMenu *subm_option = new ColumnMenu(title_option, 200);

    subm_option->AddedSubMenu("Bella", subm_Bella);
    subm_file->AddedSubMenu("Options", subm_option);

    subm_file->AddedSubMenu("Quit", subm_Bella2);
    topmenu->AddedSubMenu("File", subm_file);

    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    NameList title3 = {
        "Copy                            Ctrl+C",
        "Cut                               Ctrl+X",
        "Paste                           Ctrl+V",
        "Delete                          Delete",
        "Undo                            Ctrl+Z",
        "Redo                            Ctrl+Y"};
    ColumnMenu *subm_Edit = new ColumnMenu(title3, 200);
    __KeyList__ edit_keys= 
    {
        cmd_KEY::cmd_COPY,
        cmd_KEY::cmd_EMPTY,
        cmd_KEY::cmd_PASTE,
        cmd_KEY::cmd_Delete,
        cmd_KEY::cmd_UNDO,
        cmd_KEY::cmd_REDO
    };
    subm_Edit->AddCommand(edit_keys);

    topmenu->AddedSubMenu("Edit", subm_Edit);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    NameList title4 = {
        "Home Telepad                       H",
        "Home Tall                              H",
        "Show/Hide Console         Ctrl+;",
        "Clear Selection                       "};
    ColumnMenu *subm_View = new ColumnMenu(title4, 200);
    __KeyList__ viewtab_cmds = 
    {
        cmd_KEY::cmd_HOME_Pad,
        cmd_KEY::cmd_HOME_Tall,
        cmd_KEY::cmd_CONSOLE_SHOW,
        cmd_KEY::cmd_CLEAR_Sel
    };
    subm_View->AddCommand(viewtab_cmds);


    NameList node_title = {"Point Node"};
    __KeyList__ node_cmds = {cmd_KEY::cmd_CREATE_NODE_Point_M};
    ColumnMenu *subm_Nodes = new ColumnMenu(node_title, 200);
    subm_Nodes->AddCommand(node_cmds);

    topmenu->AddedSubMenu("Nodes", subm_Nodes);
    topmenu->AddedSubMenu("View", subm_View);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
}