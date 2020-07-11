#include "teletall_control.h"


Telecontroller::Telecontroller() : _tltlvertprop(0.5)
{

}


Telecontroller::~Telecontroller()
{}




double Telecontroller::GetSplitLocation()
{
    return _tltlvertprop;
}

void Telecontroller::SetSplitLocation(double new_prop)
{
    _tltlvertprop = new_prop;
}