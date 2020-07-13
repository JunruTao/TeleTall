#include "teletall_button.h"

size_t Button::_count = 0;

Button::Button()
{
    _name = "default_button";
    _info = " ";
    _id = "button" + std::to_string(_count);
    _count++;
}

std::string Button::GetName() const { return _name; }

std::string Button::GetInfo() const { return _info; }

std::string Button::GetID() const { return _id; }
