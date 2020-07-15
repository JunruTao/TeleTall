#include "teletall_button.h"



//----------------------------------------------------------
size_t Button::_count = 0;

Button::Button()
{
    screenPos.New(0,0);
    _info = " ";
    _id = "button" + std::to_string(_count);
    _count++;
}

std::string Button::GetName() const { return _name; }

std::string Button::GetInfo() const { return _info; }

std::string Button::GetID() const { return _id; }

void Button::SetName(std::string name) { _name = name; }

void Button::SetInfo(std::string info) { _info = info; }



//----------------------------------------------------------

MenuButton::MenuButton(Point2D<int> screenLocation, int width, std::string in_text)
{
    _text = new ScreenText;
    _name = in_text;
    this->_buttonrec.x = screenLocation.x;
    this->_buttonrec.y = screenLocation.y;
    this->_buttonrec.w = width;
    this->_buttonrec.h = _text->GetHeight()+4;
    
    _state = ButtonStates::NORMAL;
    _nor_color = {200,200,200,255};
    _pass_color = {230,230,230,255};
    _click_color = {100,100,100,255};
    _text_color = {0,0,0,255};
    
}


MenuButton::MenuButton(int x, int y, int width, std::string in_text)
{
    _text = new ScreenText;
    _name = in_text;
    this->_buttonrec.x = x;
    this->_buttonrec.y = y;
    this->_buttonrec.w = width;
    this->_buttonrec.h = _text->GetHeight()+4;
    
    _state = ButtonStates::NORMAL;
    _nor_color = {200,200,200,255};
    _pass_color = {230,230,230,255};
    _click_color = {100,100,100,255};
    _text_color = {0,0,0,255};
    
}



void MenuButton::Update(Telecontroller* controller)
{
    if (controller->GetMousePoint()->InBoundWH(_buttonrec.x, _buttonrec.y, _buttonrec.w, _buttonrec.h))
    {
        _state = ButtonStates::PASSING;
        if (controller->LMB_hold)
        {
            _state = ButtonStates::CLICKED;
        }
    }else
    {
        _state = ButtonStates::NORMAL;
    }
}


void MenuButton::Draw(SDL_Renderer* renderer)const
{
    _text->loadFromRenderedText(_name,_text_color,_text_color,renderer,3);
    switch (_state)
    {
    case ButtonStates::PASSING :
        SDL_SetRenderDrawColor(renderer,_pass_color.r,_pass_color.g,_pass_color.b,_pass_color.a);
        SDL_RenderFillRect(renderer,&_buttonrec);
        SDL_SetRenderDrawColor(renderer,_pass_color.r-10,_pass_color.g-10,_pass_color.b-10,_pass_color.a);
        SDL_RenderDrawRect(renderer,&_buttonrec);
        break;
    case ButtonStates::CLICKED:
        SDL_SetRenderDrawColor(renderer,_click_color.r,_click_color.g,_click_color.b,_click_color.a);
        SDL_RenderFillRect(renderer,&_buttonrec);
        break;
    default:
        SDL_SetRenderDrawColor(renderer,_nor_color.r,_nor_color.g,_nor_color.b,_nor_color.a);
        SDL_RenderFillRect(renderer,&_buttonrec);
        SDL_SetRenderDrawColor(renderer,_pass_color.r+10,_pass_color.g+10,_pass_color.b+10,_pass_color.a);
        SDL_RenderDrawRect(renderer,&_buttonrec);
        break;
    }
    _text->Draw(renderer,_buttonrec.x+((_buttonrec.w - _text->GetWidth())/2),_buttonrec.y+2);
}