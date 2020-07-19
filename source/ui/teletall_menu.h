#ifndef TELETALL_MENU_H
#define TELETALL_MENU_H

#include "teletall_button.h"
#include "teletall_graphics.h"
#include <map>
#include <unordered_map>

class Menu
{
    public:
    virtual void Draw(SDL_Renderer* renderer) const= 0;
};



class ColumnMenu : public Menu
{
    public:
    ColumnMenu(std::vector<std::string> options, int button_w);
    ColumnMenu(){};
    ~ColumnMenu();
    void Update(Telecontroller* controller, int x, int y, bool show);
    void Draw(SDL_Renderer* renderer) const override;
    void AddedSubMenu(std::string target_name, ColumnMenu* submenu_to_add);

    private:
    std::vector<MenuButton*> MenuItem;
    std::string active;
    std::map<std::string, ColumnMenu*> Menu_Map;
    Point2D<int> _initPos;
    SDL_Rect _menurec;
    int _Height;
    bool _ifshown;

};



class BarMenu : public Menu
{
    public:
    BarMenu(SDL_Window* hwnd ,std::vector<std::string> options, int button_w);
    ~BarMenu();

    void Update(Telecontroller* controller);
    void Draw(SDL_Renderer* renderer) const override;
    void AddedSubMenu(std::string target_name, ColumnMenu* submenu_to_add);

    private:
    std::vector<MenuButton*> MenuItem;
    std::map<std::string, ColumnMenu*> Menu_Map;
    std::string active;
    SDL_Rect _menurec;
    int _unitwidth;
    int number_opened;
};


#endif