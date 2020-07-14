#ifndef JUTA_GEOMETRY_CORE_H
#define JUTA_GEOMETRY_CORE_H
#include <cmath>
#include <string>

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
        double len = sqrt(pow((double)vec2.x, 2) + pow((double)vec2.y, 2));
        return len;
    }

    double Length()
    {
        double len = sqrt(pow((double)this->x, 2) + pow((double)this->y, 2));
        return len;
    }

    void Translate(const Point2D &vec2)
    {
        this->x += vec2.x;
        this->y += vec2.y;
    }

    bool InBound(const var &minx, const var &miny, const var &maxx, const var &maxy)
    {
        return (this->x > minx) && (this->x < maxx) && (this->y > miny) && (this->y < maxy);
    }

    void New(const var &_x, const var &_y)
    {
        this->x = _x;
        this->y = _y;
    }
};

template <class var>
struct Line2D
{

public:
    Line2D(const Point2D<var> &a, const Point2D<var> &b)
    {
        _start = new Point2D<var>(a.x, a.y);
        _end = new Point2D<var>(b.x, b.y);
        _ref_start = nullptr;
        _ref_start = nullptr;
        _isRefType = false;
    }
    Line2D(Point2D<var> *ref_a, Point2D<var> *ref_b)
    {
        _start = nullptr;
        _end = nullptr;
        _ref_start = &ref_a;
        _ref_end = &ref_b;
        _isRefType = true;
    }

    ~Line2D()
    {
        if (_isRefType)
        {
            _ref_start = nullptr;
            _ref_end = nullptr;
        }
        else
        {
            delete _start;
            delete _end;
        }
    }


    std::string GetType()
    {
        if (_isRefType)
        {
            return "Line2D_REF";
        }
        else
        {
            return "Line2D_STD";
        }
    }

private:
    //field
    Point2D<var> *_start;
    Point2D<var> *_end;

    Point2D<var> **_ref_start;
    Point2D<var> **_ref_end;
    
    /*In order to manipulate at point level, that points passed from
    a higher level, this container holds pointer of point2D pointers 

                      [Point] [Point] ____{drawing}
                           |  |
                           |  |
                          (a)(b)
                         [ Line ]
                            |
                           /|______{drawing}
                          /
                         |
                [next node container]________{drawing}
    */
    bool _isRefType;
};

#endif