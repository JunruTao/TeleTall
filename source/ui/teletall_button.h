#ifndef TELETALL_BUTTON_H
#define TELETALL_BUTTON_H

#include "x_sdl2headers.h"
#include <string>
#include "ui/teletall_graphics.h"
#include "JUTA/JUTA_geometry_core.h"
#include "JUTA/JUTA_math.h"
#include "teletall_control.h"


enum class ButtonStates{NORMAL, PASSING, CLICKED, OPENED};


class Button
{
public:
    Button();

    void SetName(std::string name);
    void SetInfo(std::string info);

    std::string GetName() const;
    std::string GetInfo() const;
    std::string GetID() const;

    virtual void Draw(SDL_Renderer* renderer) = 0;

    Point2D<int> screenPos;

protected:
    std::string _name;
    std::string _id;
    std::string _info;

    static size_t _count;
};




class MenuButton : public Button
{
public:
    MenuButton(Point2D<int> screenLocation, int width, std::string in_text, bool centered, cmd_KEY cmd);
    MenuButton(int x, int y, int width, std::string in_text,bool centered);
    ~MenuButton(){delete _text;};

    void Draw(SDL_Renderer* renderer) override;
    void Update(Telecontroller* controller);
    void PositionOffset(int offset_x, int offset_y);

    ButtonStates GetState() const{ return _state; }
    void ReleaseState() {_state = ButtonStates::NORMAL;}
    void SetOpenState() {_state = ButtonStates::OPENED;}
    int GetButtonX(){return _buttonrec.x;}
    int GetButtonY(){return _buttonrec.y;}
    int SetCommand(cmd_KEY cmd){ _buttom_cmd = cmd;}

    int GetButtonWidth(){return _buttonrec.w;}
    int GetButtonHeight(){return _buttonrec.h;}
    void HaveFunctionTrue(){_have_function = true;}
    void HaveChildTrue(){_have_child = true;}
    void ShouldRest(){_shouldrest = true;}

private:
    Point2D<int> _initPos;
    ScreenText* _text;
    SDL_Rect _buttonrec;
    ButtonStates _state;
    SDL_Color _nor_color;
    SDL_Color _pass_color;
    SDL_Color _click_color;
    SDL_Color _text_color;
    bool _if_centered;
    bool _have_child;
    bool _have_function;
    bool _opened;
    bool _shouldrest;
    void InitButtonColors();
    cmd_KEY _buttom_cmd;
};

#endif