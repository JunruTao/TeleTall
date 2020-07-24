#ifndef TELETALL_CONSOLE_H
#define TELETALL_CONSOLE_H

#include "teletall_graphics.h"
#include <unordered_map>
#include <string>
#include <vector>


enum class cmd_KEY
{
    cmd_EMPTY,
    cmd_STARTUP,cmd_RMB,cmd_LMB,cmd_MMB,
    cmd_HOME,cmd_FRAME,cmd_FILE,cmd_SAVE,cmd_NEW,
    cmd_UNDO, cmd_REDO,cmd_COPY,cmd_PASTE, cmd_CUT, cmd_Delete,
    cmd_WINDOWSIZE,cmd_CONSOLE_SHOW, cmd_HOME_Pad, 
    cmd_HOME_Tall, cmd_WIN_ABOUT, cmd_CLEAR_Sel,
    cmd_CREATE_NODE_Point, cmd_CREATE_NODE_Point_M,
    cmd_CREATE_POINT, cmd_GRID_SNAP_M,
    cmd_EditMode, cmd_EditMode_M, cmd_DisplayFlag, 
    cmd_Message, cmd_QUIT, cmd_ZOOM_IN, cmd_ZOOM_OUT
    
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