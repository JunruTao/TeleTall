#include "JUTA/JUTA_geometry_core.h"



int GeoData::selection_color_grayscale = 60;

//_________________________________________________ 1
//Point3D constructor
Point3D::Point3D()
{
    _pos = std::make_unique<Vector3>(0,0,0);
}

//_________________________________________________ 2
//Point3D constructor
Point3D::Point3D(double x, double y, double z)
{
    _pos = std::make_unique<Vector3>(x,y,z);
}

//_________________________________________________ 3
//Point3D constructor
Point3D::Point3D(const Vector3& pos)
{
    _pos = std::make_unique<Vector3>(pos.x,pos.y,pos.z);
}

//_________________________________________________ 4 
//Point3D constructor
Point3D::Point3D(Vector3&& pos)
{
    _pos = std::make_unique<Vector3>(pos.x,pos.y,pos.z);
}




//_________________________________________________ 5
//copy constructor(deep copies)
Point3D::Point3D(const Point3D& point)
{
    this->_pos->x = point._pos->x;
    this->_pos->y = point._pos->y;
    this->_pos->z = point._pos->z;

    this->_color = point._color;
}

//_________________________________________________ 6
//assignment copy operator(deep copies)
Point3D& Point3D::operator=(const Point3D& point)
{
    if(this == &point){return *this;}
    else
    {
        this->_pos->x = point._pos->x;
        this->_pos->y = point._pos->y;
        this->_pos->z = point._pos->z;

        _color = point._color;
        return *this;
    }
}


//_________________________________________________ 7
//move constructor(deep copies)
Point3D::Point3D(Point3D&& point)
{
    this->_pos = std::move(point._pos);
    _color = point._color;
}

//_________________________________________________ 8
//move assignment copy operator(deep copies)
Point3D& Point3D::operator=(Point3D&& point)
{
    if(this == &point){return *this;}
    else
    {
        this->_pos = std::move(point._pos);
        _color = point._color;
        return *this;
    }

}

//_________________________________________________ 9
//Destructor
Point3D::~Point3D(){}



//_________________________________________________ 10
//Draw Point geo:
void Point3D::Draw(SDL_Renderer *renderer, Point2D<int> origin, int gridscale, int mode)
{
    if(mode == 1){
        SDL_SetRenderDrawColor(renderer, _color.r, _color.g, _color.b, _color.a);
    }
    else
    {
        SDL_SetRenderDrawColor(renderer, selection_color_grayscale, selection_color_grayscale, selection_color_grayscale, _color.a);
    }
    int x = (int)(((double)origin.x) + ((double)gridscale) * _pos->x);
    int y = (int)(((double)origin.y) - ((double)gridscale) * _pos->y);

    SDL_RenderDrawLine(renderer, x - 4, y - 4, x + 4 , y + 4);
    SDL_RenderDrawLine(renderer, x - 4, y + 4, x + 4 , y - 4);
    SDL_RenderDrawLine(renderer, x - 4+1, y - 4, x + 4+1 , y + 4);
    SDL_RenderDrawLine(renderer, x - 4+1, y + 4, x + 4+1 , y - 4);
    SDL_RenderDrawLine(renderer, x - 4-1, y - 4, x + 4-1 , y + 4);
    SDL_RenderDrawLine(renderer, x - 4-1, y + 4, x + 4-1 , y - 4);

}


//Point's Features: 
//_________________________________________________ 11
//Translate
void Point3D::Translate(const double& x,const double& y,const double& z)
{
    _pos->x += x;
    _pos->y += y;
    _pos->z += z;
}

//_________________________________________________ 12
//Translate
void Point3D::Translate(const Vector3& motion)
{
    _pos->x += motion.x;
    _pos->y += motion.y;
    _pos->z += motion.z;
}

//_________________________________________________ 13
//Translate
void Point3D::Translate(Vector3&& motion)
{
    _pos->x += motion.x;
    _pos->y += motion.y;
    _pos->z += motion.z;
}