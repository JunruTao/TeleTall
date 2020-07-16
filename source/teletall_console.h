#ifndef TELETALL_CONSOLE_H
#define TELETALL_CONSOLE_H

#include "teletall_graphics.h"
#include <unordered_map>
#include <string>
#include <vector>


enum class cmd_KEY
{
    cmd_STARTUP,cmd_RMB,cmd_LMB,cmd_MMB,cmd_EMPTY,
    cmd_HOME,cmd_FRAME,cmd_FILE,cmd_SAVE,cmd_NEW,
    cmd_WINDOWSIZE,cmd_CONSOLE_SHOW
};

class Console
{
public:
    Console();
    ~Console();
    void Update(cmd_KEY cmd_key,std::string msg = "");
    void Draw(SDL_Window** hwnd, SDL_Renderer* renderer);
    int GetHeight();
    bool IsShown();

private:
    void Init();
    std::unordered_map<cmd_KEY,std::string> cmd_dictionary;
    std::vector<ScreenText* > _text_Objects;
    std::vector<std::string> _texts;
    size_t _linesize;
    int _console_win_height;
    SDL_Rect _textwindowrec;  
    SDL_Color _textcolor;  
    bool _showing;
};





#endif