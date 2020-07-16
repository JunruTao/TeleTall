#include "teletall_menu.h"


BarMenu::BarMenu(SDL_Window* hwnd, std::vector<std::string> options, int button_w)
{
    int h; int w;
    SDL_GetWindowSize(hwnd,&w,&h);
    _menurec = {0, 0, w, h};
    for(int i = 0; i < options.size(); i++)
    {
        MenuItem.emplace_back(new MenuButton( 3 + ((button_w + 2) * i) , 3, button_w, options[i],true));
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

void BarMenu::Update(Telecontroller* controller)
{
    
    _menurec.h = ScreenText::GetUniversalTextHeight() + 10;
    int h;
    SDL_GetWindowSize(controller->GetHWND(), &_menurec.w, &h);
    for(auto &b: MenuItem)
    {
        b->Update(controller);
    }

    if(controller->GetMousePoint()->InBound(0,0,_menurec.w,_menurec.h))
    {
        controller->current_panel = PanelID::ON_MENU;
    }
    controller->LinkMenuRec(&_menurec);
}

void BarMenu::Draw(SDL_Renderer* renderer)const
{
    SDL_SetRenderDrawColor(renderer, 55,55,55,255);
    SDL_RenderFillRect(renderer,&_menurec);
    for(auto &b: MenuItem)
    {
        b->Draw(renderer);
    }
}






//==================================================
//ColumnMenu:
ColumnMenu::~ColumnMenu()
{
    if(!MenuItem.empty())
    {
        for (auto &e : MenuItem)
        {
            delete e;
        }      
    }
}

ColumnMenu::ColumnMenu(std::vector<std::string> options, int button_w, void* parent, std::string parent_name)
{
    int h = 16;
    int x = 30;
    int y = 30;
    _menurec = {x, y, button_w, (int)((h+5) * (int)options.size())};
    for(int i = 0; i < options.size(); i++)
    {
        MenuItem.emplace_back(new MenuButton( x+3 , 3+y + i*(h+4), button_w-6, options[i],false));
    }
    hParent = parent;
}


void ColumnMenu::Update(Telecontroller* controller)
{
    for(auto &b: MenuItem)
    {
        b->Update(controller);
    }

    //if(controller->GetMousePoint()->InBound(0,0,_menurec.w,_menurec.h))
    //{
        controller->current_panel = PanelID::ON_MENU;
    //}
}

void ColumnMenu::Draw(SDL_Renderer* renderer)const
{
    SDL_SetRenderDrawColor(renderer, 55,55,55,255);
    SDL_RenderFillRect(renderer,&_menurec);
    for(auto &b: MenuItem)
    {
        b->Draw(renderer);
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 20,20,20,100);
    SDL_RenderDrawLine(renderer,_menurec.x-1,_menurec.y,_menurec.x-1,_menurec.y+_menurec.h);
    SDL_RenderDrawLine(renderer,_menurec.x-1,_menurec.y+1+_menurec.h,_menurec.x-1+_menurec.w,_menurec.y+1+_menurec.h);
    SDL_SetRenderDrawColor(renderer, 20,20,20,50);
    SDL_RenderDrawLine(renderer,_menurec.x-2,_menurec.y,_menurec.x-2,_menurec.y+_menurec.h);
    SDL_RenderDrawLine(renderer,_menurec.x-1,_menurec.y+_menurec.h+2,_menurec.x-1+_menurec.w,_menurec.y+2+_menurec.h);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

}
