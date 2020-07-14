#ifndef TELETALL_BUTTON_H
#define TELETALL_BUTTON_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include "JUTA/JUTA_geometry_core.h"
#include "JUTA/JUTA_math.h"

class Button
{
public:
    Button();

    void SetName(std::string name);
    void SetInfo(std::string info);

    std::string GetName() const;
    std::string GetInfo() const;
    std::string GetID() const;

    virtual void Draw() const = 0;

    Point2D<int> screenPos;

protected:
    std::string _name;
    std::string _id;
    std::string _info;

    static size_t _count;
};

class MenuButton : public Button
{
    void Draw()const override;
};

#endif