#ifndef TELETALL_MENU_H
#define TELETALL_MENU_H

#include "teletall_button.h"
#include "teletall_graphics.h"

class Menu
{
    public:
    virtual void Draw(SDL_Renderer* renderer) const= 0;
    virtual void Update(Telecontroller* controller) = 0;
};



class ColumnMenu : public Menu
{
    public:
    ColumnMenu();
    ~ColumnMenu();
    private:
    std::vector<MenuButton*> MenuItem;

};



class BarMenu : public Menu
{
    public:
    BarMenu(SDL_Window* hwnd ,std::vector<std::string> options, int button_w);
    ~BarMenu();

    private:
    std::vector<MenuButton*> MenuItem;
    SDL_Rect _menurec;
    int _unitwidth;
};


#endif