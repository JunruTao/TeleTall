#ifndef TELETALL_CONTROL_H
#define TELETALL_CONTROL_H

class Telecontroller
{
public:
    Telecontroller();
    ~Telecontroller();
    
    double GetSplitLocation();
    void SetSplitLocation(double new_prop);
private:
    double _tltlvertprop;
};

#endif