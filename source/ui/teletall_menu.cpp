#include "teletall_menu.h"


BarMenu::BarMenu(SDL_Window* hwnd, std::vector<std::string> options, int button_w)
{
    int h; int w;
    SDL_GetWindowSize(hwnd,&w,&h);
    _menurec = {0, 0, w, h};
    _menurec.h = ScreenText::GetUniversalTextHeight() + 6;
    for(int i = 0; i < options.size(); i++)
    {
        MenuItem.emplace_back(new MenuButton( 2, 2+(button_w * i), button_w, options[i],true));
    }
}

BarMenu::~BarMenu()
{
    if(!MenuItem.empty())
    {
        for (auto &e : MenuItem)
        {
            delete e;
        }
        
    }
}