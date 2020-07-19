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





//Set Colors here--------------------
void MenuButton::InitButtonColors()
{
    _state = ButtonStates::NORMAL;
    _have_child = false;
    _have_function = false;
    _opened = false;
    _shouldrest = false;
    _nor_color =   {66,66,66,255};
    _pass_color =  {80,80,80,80};
    _click_color = {10,10,10,255};
    _text_color =  {200,  200,  200,  255};
    _text = new ScreenText;
    
}




MenuButton::MenuButton(Point2D<int> screenLocation, int width, std::string in_text, bool centered, cmd_KEY cmd)
{
    _name = in_text;
    this->_buttonrec.x = screenLocation.x;
    this->_buttonrec.y = screenLocation.y;
    this->_buttonrec.w = width;
    this->_buttonrec.h = ScreenText::GetUniversalTextHeight()+4;
    _if_centered = centered;
    InitButtonColors();
    _buttom_cmd = cmd;
    _initPos = screenLocation;
}


MenuButton::MenuButton(int x, int y, int width, std::string in_text, bool centered)
{
    _name = in_text;
    this->_buttonrec.x = x;
    this->_buttonrec.y = y;
    this->_buttonrec.w = width;
    this->_buttonrec.h = _text->ScreenText::GetUniversalTextHeight()+4;
    _if_centered = centered;
    InitButtonColors();
    _initPos.New(x,y);
}



void MenuButton::Update(Telecontroller* controller)
{
    if (_shouldrest)
    {
        _state = ButtonStates::NORMAL;
        _opened = false;
        goto skipping;
    }
    if (controller->GetMousePoint()->InBoundWH(_buttonrec.x, _buttonrec.y, _buttonrec.w, _buttonrec.h))
    {
        if (_have_child)
        {
            if (_opened)
            {
                _state = ButtonStates::OPENED;
            }
            else
            {
                _state = ButtonStates::PASSING;
            }

            if (controller->GetCommand() == cmd_KEY::cmd_LMB)
            {
                _state = ButtonStates::CLICKED;
                _opened = !_opened;
            }
        }
        else
        {
            _state = ButtonStates::PASSING;
            if (controller->GetCommand() == cmd_KEY::cmd_LMB)
            {
                _state = ButtonStates::CLICKED;
            }
        }
    }
    else
    {
        if(_opened && _have_child)
        {
            _state = ButtonStates::OPENED;
        }
        else if(_have_child)
        {
            _state = ButtonStates::NORMAL;
        }else
        {
            _state = ButtonStates::NORMAL;
        }

        if (controller->GetCommand() == cmd_KEY::cmd_LMB || controller->GetCommand() == cmd_KEY::cmd_RMB || controller->GetCommand() == cmd_KEY::cmd_MMB )
        {
            
            if (_opened && _have_child && controller->current_panel == PanelID::ON_MENU)
            {
                _state = ButtonStates::OPENED;
                _opened = true;
                
            }
            else
            {
                _state = ButtonStates::NORMAL;
                _opened = false;
            }
            skipping:
            {

            }
        }
    }
    _shouldrest = false;
}

void MenuButton::PositionOffset(int offset_x, int offset_y)
{
    this->_buttonrec.x = _initPos.x + offset_x;
    this->_buttonrec.y = _initPos.y + offset_y;
}



void MenuButton::Draw(SDL_Renderer* renderer)
{
    _text->loadFromRenderedText(_name,renderer,_text_color,_text_color,3);
    //if(true)_buttonrec.w = _text->GetWidth() + 10;
    switch (_state)
    {
    case ButtonStates::PASSING :
        SDL_SetRenderDrawColor(renderer,_pass_color.r,_pass_color.g,_pass_color.b,_pass_color.a);
        SDL_RenderFillRect(renderer,&_buttonrec);
        SDL_SetRenderDrawColor(renderer,_pass_color.r-10,_pass_color.g-10,_pass_color.b-10,_pass_color.a);
        SDL_RenderDrawRect(renderer,&_buttonrec);
        break;
    case ButtonStates::OPENED: 
        SDL_SetRenderDrawColor(renderer,_click_color.r,_click_color.g,_click_color.b,_click_color.a);
        SDL_RenderFillRect(renderer,&_buttonrec);
        break;
    default:
        SDL_SetRenderDrawColor(renderer,_nor_color.r,_nor_color.g,_nor_color.b,_nor_color.a);
        SDL_RenderFillRect(renderer,&_buttonrec);
        break;
    }
    if (_if_centered)
    {
        _text->Draw(renderer, _buttonrec.x + ((_buttonrec.w - _text->GetWidth()) / 2 + 2), _buttonrec.y + ((_buttonrec.h - _text->GetHeight()) / 2), 0.87);
    }
    else
    {
        _text->Draw(renderer, _buttonrec.x + 2, _buttonrec.y + ((_buttonrec.h - _text->GetHeight()) / 2),0.94);
    }
}