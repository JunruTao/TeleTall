#ifndef TELETALL_MENU_H
#define TELETALL_MENU_H

#include "teletall_button.h"

class Menu
{
public:
    Menu();
    ~Menu();

protected:
    Menu* _parent;
    std::string _name;
};



class ColumnMenu : public Menu
{
    public:
    ColumnMenu();
    ~ColumnMenu();

    private:

};

class BarMenu : public Menu
{
    public:
    BarMenu();
    ~BarMenu();

    private:
    
};


#endif