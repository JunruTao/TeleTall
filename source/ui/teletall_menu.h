#ifndef TELETALL_MENU_H
#define TELETALL_MENU_H

#include "teletall_button.h"
#include "teletall_graphics.h"
#include <map>

class Menu
{
    public:
    virtual void Draw(SDL_Renderer* renderer) const= 0;
    virtual void Update(Telecontroller* controller) = 0;
};



class ColumnMenu : public Menu
{
    public:
    ColumnMenu(std::vector<std::string> options, int button_w, void* parent, std::string parent_name);
    ~ColumnMenu();
    void Update(Telecontroller* controller)override;
    void Draw(SDL_Renderer* renderer) const override;
    
    void Show(){};

    private:
    void* hParent;
    std::vector<MenuButton*> MenuItem;
    std::map<std::string, Menu*> Menu_Map;
    SDL_Rect _menurec;
    int _Height;

};



class BarMenu : public Menu
{
    public:
    BarMenu(SDL_Window* hwnd ,std::vector<std::string> options, int button_w);
    ~BarMenu();

    void Update(Telecontroller* controller)override;
    void Draw(SDL_Renderer* renderer) const override;
    void AddedSubMenu(std::string, Menu*);

    private:
    std::vector<MenuButton*> MenuItem;
    std::map<std::string, Menu*> Menu_Map;
    SDL_Rect _menurec;
    int _unitwidth;
};


#endif