#ifndef JUTA_GEOMETRY_CORE_H
#define JUTA_GEOMETRY_CORE_H
#include <cmath>
template <class var>
struct Point2D
{
    Point2D(var _x, var _y) : x(_x), y(_y){};
    Point2D() : x(0), y(0){};
    var x;
    var y;

    Point2D operator+(const Point2D &a)
    {
        Point2D pt;
        pt.x = this->x + a.x;
        pt.y = this->y + a.y;
        return pt;
    }
    Point2D operator-(const Point2D &a)
    {
        Point2D pt;
        pt.x = this->x - a.x;
        pt.y = this->y - a.y;
        return pt;
    };

    double Length(const Point2D &vec2)
    {
        double len = sqrt(pow((double)vec2.x,2) + pow((double)vec2.y,2));
        return len;
    }
    
    double Length()
    {
        double len = sqrt(pow((double)this->x,2) + pow((double)this->y,2));
        return len;
    }

    void Translate(const Point2D &vec2)
    {
        this->x += vec2.x;
        this->y += vec2.y;
    }
};

#endif