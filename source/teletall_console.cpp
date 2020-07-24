#include "teletall_console.h"

#define CONSOLE_LINE_COUNT 6

void Console::Init()
{
    cmd_dictionary[cmd_KEY::cmd_HOME] = "> [H] Viewport: center viewport";
    cmd_dictionary[cmd_KEY::cmd_HOME_Pad] = "> [H] Viewport: center TelePad-viewport";
    cmd_dictionary[cmd_KEY::cmd_HOME_Tall] = "> [H] Viewport: center Tall-viewport";
    cmd_dictionary[cmd_KEY::cmd_FRAME] = "> [F] Viewport: Framing Selection";
    cmd_dictionary[cmd_KEY::cmd_WINDOWSIZE] = "> WINDOW: changed size";
    cmd_dictionary[cmd_KEY::cmd_LMB] = "> Mouse: left mouse clicked";
    cmd_dictionary[cmd_KEY::cmd_RMB] = "> Mouse: Right mouse clicked";
    cmd_dictionary[cmd_KEY::cmd_MMB] = "> Mouse: Middle mouse clicked";

    cmd_dictionary[cmd_KEY::cmd_UNDO] = "> Edit: Undo";
    cmd_dictionary[cmd_KEY::cmd_REDO] = "> Edit: Redo";
    cmd_dictionary[cmd_KEY::cmd_Delete] = "> Edit: Delete";
    cmd_dictionary[cmd_KEY::cmd_COPY] = "> Edit: Copy";
    cmd_dictionary[cmd_KEY::cmd_PASTE] = "> Edit: Paste";
    cmd_dictionary[cmd_KEY::cmd_CUT] = "> Edit: Cut";
    cmd_dictionary[cmd_KEY::cmd_EditMode] = "> Edit: ";
    cmd_dictionary[cmd_KEY::cmd_EditMode_M] = "> Edit: ";
    cmd_dictionary[cmd_KEY::cmd_Message] = "> ";
    cmd_dictionary[cmd_KEY::cmd_WIN_ABOUT] = "> About: software information";

    cmd_dictionary[cmd_KEY::cmd_STARTUP] = "> Teletall: Program Init: SUCCESS - ";
    cmd_dictionary[cmd_KEY::cmd_CREATE_NODE_Point] = "> Create Node: Point Node";
    cmd_dictionary[cmd_KEY::cmd_CREATE_NODE_Point_M] = "> Create Node: Point Node";
    cmd_dictionary[cmd_KEY::cmd_CREATE_POINT] = "> Create Point ";
    
}

Console::Console()
{
    Init();
    _showing = true;
    _linesize = CONSOLE_LINE_COUNT;
    _console_win_height = 30;
    _textcolor = {0,0,0,255};
    _textwindowrec.h = _linesize * ScreenText::GetUniversalTextHeight() + 6; //6 is upper+down buffer
}

Console::~Console()
{
    //freeing the text memory
    if(!_text_Objects.empty())
    {
        for(auto &e:_text_Objects)
        {
            delete e;
        }
    }
}


void Console::Update(cmd_KEY command, std::string msg)
{
    if(command == cmd_KEY::cmd_CONSOLE_SHOW)
    {
        _showing = !_showing;
        if(_showing)
        {
            _texts.push_back("> [ctrl + ; ] Console: cmd-show console panel");
        }else
        {
            _texts.push_back("> [ctrl + ; ] Console: cmd-hide console panel");
        }
    }
    else if(cmd_dictionary.find(command) != cmd_dictionary.end())
    {
        _texts.push_back(cmd_dictionary[command]);
        if(!msg.empty())
        {
            _texts[_texts.size()-1] += "[message: " + msg +  "]";
        }
    }
    if(_texts.size()>_linesize)
    {
        _texts.erase(_texts.begin());
    }
}

void Console::Draw(SDL_Window** hwnd, SDL_Renderer* renderer)
{
    if (_showing)
    {
        if (!_texts.empty())
        {
            for (int i = 0; i < _texts.size(); i++)
            {
                _text_Objects.emplace_back(new ScreenText());
                _text_Objects[i]->loadFromRenderedText(_texts[i], renderer, _textcolor, _textcolor, 3);
            }
        }
        int w, h;
        SDL_GetWindowSize(*hwnd, &w, &h);
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        _textwindowrec.x = 0;
        _textwindowrec.y = h - _textwindowrec.h;
        _textwindowrec.w = w;
        SDL_RenderFillRect(renderer, &_textwindowrec);

        SDL_SetRenderDrawColor(renderer, 90, 90, 90, 255);
        SDL_RenderDrawLine(renderer, 0, _textwindowrec.y - 1, _textwindowrec.w, _textwindowrec.y - 1);
        SDL_RenderDrawLine(renderer, 0, _textwindowrec.y - 5, _textwindowrec.w, _textwindowrec.y - 5);
        SDL_SetRenderDrawColor(renderer, 110, 110, 110, 255);
        SDL_RenderDrawLine(renderer, 0, _textwindowrec.y - 2, _textwindowrec.w, _textwindowrec.y - 2);
        SDL_RenderDrawLine(renderer, 0, _textwindowrec.y - 4, _textwindowrec.w, _textwindowrec.y - 4);
        SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255);
        SDL_RenderDrawLine(renderer, 0, _textwindowrec.y - 3, _textwindowrec.w, _textwindowrec.y - 3);

        for (int i = 0; i < _text_Objects.size(); i++)
        {
            _text_Objects[i]->Draw(renderer, _textwindowrec.x + 10, _textwindowrec.y + 2 + (_text_Objects[i]->GetHeight()) * i, 0.8);
        }
    }
}


int Console::GetHeight()
{
    return _textwindowrec.h + 5;
}
bool Console::IsShown()
{
    return _showing;
}