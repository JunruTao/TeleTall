#ifndef TELETALL_CURSOR_H
#define TELETALL_CURSOR_H


#include <sstream>
#include <SDL2/SDL.h>

/* XPM */
static const char *doublearrow[] = {
    /* width height num_colors chars_per_pixel */
    "    32    32        3            1",
    /* colors */
    "X c #aeaeae",
    ". c #ffffff",
    "  c None",
    /* pixels */
    "                                ", //01
    "                                ", //02
    "                                ", //03
    "                                ", //04
    "                                ", //05
    "                                ", //06
    "                                ", //07
    "                                ", //08
    "                                ", //09
    "                                ", //10
    "     X                    X     ", //11
    "    XX                    XX    ", //12
    "   X.X                    X.X   ", //13
    "  X..X                    X..X"  , //14
    " X...XXXXXXXXXX  XXXXXXXXXX...X ", //15
    "X.............X  X.............X", //16
    " X...XXXXXXXXXX  XXXXXXXXXX...X ", //17
    "  X..X                    X..X  ", //18
    "   X.X                    X.X   ", //19
    "    XX                    XX    ", //20
    "     X                    X     ", //21
    "                                ", //22
    "                                ", //23
    "                                ", //24
    "                                ", //25
    "                                ", //26
    "                                ", //27
    "                                ", //28
    "                                ", //29
    "                                ", //30
    "                                ", //31
    "                                ", //32
    "0,0"};


 
/* XPM */
static const char *crossx[] = {
    /* width height num_colors chars_per_pixel */
    "    32    32        3            1",
    /* colors */
    "X c #000000",
    ". c #ffffff",
    "  c None",
    /* pixels */
    "               X                ", //01
    "              X.X               ", //02
    "             X...X              ", //03
    "            X.....X             ", //04
    "           X.......X            ", //05
    "          XXXXX.XXXXX           ", //06
    "              X.X               ", //07
    "              X.X               ", //08
    "              X.X               ", //09
    "              X.X               ", //10
    "     X        X.X         X     ", //11
    "    XX        X.X         XX    ", //12
    "   X.X        XXX         X.X   ", //13
    "  X..X                    X..X"  , //14
    " X...XXXXXXXXXX  XXXXXXXXXX...X ", //15
    "X.............X  X.............X", //16
    " X...XXXXXXXXXX  XXXXXXXXXX...X ", //17
    "  X..X                    X..X  ", //18
    "   X.X        XXX         X.X   ", //19
    "    XX        X.X         XX    ", //20
    "     X        X.X         X     ", //21
    "              X.X               ", //22
    "              X.X               ", //23
    "              X.X               ", //24
    "              X.X               ", //25
    "          XXXXX.XXXXX           ", //26
    "           X.......X            ", //27
    "            X.....X             ", //28
    "             X...X              ", //29
    "              X.X               ", //31
    "               X                ", //32
    "0,0"};



/* XPM */
static const char *cross[] = {
    /* width height num_colors chars_per_pixel */
    "    32    32        3            1",
    /* colors */
    "X c #000000",
    ". c #ffffff",
    "  c None",
    /* pixels */
    "              XXX               ", //01
    "              X.X               ", //02
    "              X.X               ", //03
    "              X.X               ", //04
    "              X.X               ", //05
    "              X.X               ", //06
    "              X.X               ", //07
    "              X.X               ", //08
    "              X.X               ", //09
    "              XXX               ", //10
    "               X                ", //11
    "                                ", //12
    "                                ", //13
    "                                ", //14
    "XXXXXXXXXXX         XXXXXXXXXXXX", //15
    "X.........XX       XX..........X", //16
    "XXXXXXXXXXX         XXXXXXXXXXXX", //17
    "                                ", //18
    "                                ",
    "                                ",
    "               X                ",
    "              XXX               ",
    "              X.X               ",
    "              X.X               ",
    "              X.X               ",
    "              X.X               ",
    "              X.X               ",
    "              X.X               ",
    "              X.X               ",
    "              X.X               ",
    "              X.X               ",
    "              XXX               ",
    "0,0"};



static SDL_Cursor *init_system_cursor(const char *image[])
{
    int i, row, col;
    Uint8 data[4 * 32];
    Uint8 mask[4 * 32];
    int hot_x, hot_y;

    i = -1;
    for (row = 0; row < 32; ++row)
    {
        for (col = 0; col < 32; ++col)
        {
            if (col % 8)
            {
                data[i] <<= 1;
                mask[i] <<= 1;
            }
            else
            {
                ++i;
                data[i] = mask[i] = 0;
            }
            switch (image[4 + row][col])
            {
            case 'X':
                data[i] |= 0x01;
                mask[i] |= 0x01;
                break;
            case '.':
                mask[i] |= 0x01;
                break;
            case ' ':
                break;
            }
        }
    }
    sscanf(image[4 + row], "%d,%d", &hot_x, &hot_y);
    return SDL_CreateCursor(data, mask, 32, 32, hot_x + 16, hot_y + 16);
}




#endif