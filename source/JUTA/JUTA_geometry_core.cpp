#include "JUTA/JUTA_geometry_core.h"



int GeoData::selection_color_grayscale = 60;

//_________________________________________________ 1
//Point3D constructor
Point3D::Point3D() : _selected(false), _passing(false)
{
    _pos = std::make_unique<Vector3>(0,0,0);
}

//_________________________________________________ 2
//Point3D constructor
Point3D::Point3D(double x, double y, double z) : _selected(false), _passing(false)
{
    _pos = std::make_unique<Vector3>(x,y,z);
}

//_________________________________________________ 3
//Point3D constructor
Point3D::Point3D(const Vector3& pos) : _selected(false), _passing(false)
{
    _pos = std::make_unique<Vector3>(pos.x,pos.y,pos.z);
}

//_________________________________________________ 4 
//Point3D constructor
Point3D::Point3D(Vector3&& pos) : _selected(false), _passing(false)
{
    _pos = std::make_unique<Vector3>(pos.x,pos.y,pos.z);
}




//_________________________________________________ 5
//copy constructor(deep copies)
Point3D::Point3D(const Point3D& point) : _selected(false), _passing(false)
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
        this->_selected = false;
        this->_passing = false;

        _color = point._color;
        return *this;
    }
}


//_________________________________________________ 7
//move constructor(deep copies)
Point3D::Point3D(Point3D&& point) : _selected(false), _passing(false)
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
        this->_selected = false;
        this->_passing = false;
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
    else if(mode == 2)
    {
        SDL_SetRenderDrawColor(renderer, 255,255, 255, _color.a);
    }
    else
    {
        SDL_SetRenderDrawColor(renderer, selection_color_grayscale, selection_color_grayscale, selection_color_grayscale, _color.a);
    }
    int x = ScreenTransformX(_pos->x,origin,gridscale);
    int y = ScreenTransformY(_pos->y,origin,gridscale);

    if (_passing && mode == 2)
    {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 255, 138, 150);
        if (_selected)
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 230);
        }
        SDL_Rect rect = {x - 9, y - 9, 19, 19};
        SDL_Rect rect2 = {x - 4, y-4, 9,9};
        SDL_RenderDrawRect(renderer, &rect);

        SDL_RenderFillRect(renderer, &rect2);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    }
    else
    {
        SDL_RenderDrawLine(renderer, x - 4, y - 4, x + 4, y + 4);
        SDL_RenderDrawLine(renderer, x - 4, y + 4, x + 4, y - 4);
        SDL_RenderDrawLine(renderer, x - 4 + 1, y - 4, x + 4 + 1, y + 4);
        SDL_RenderDrawLine(renderer, x - 4 + 1, y + 4, x + 4 + 1, y - 4);
        SDL_RenderDrawLine(renderer, x - 4 - 1, y - 4, x + 4 - 1, y + 4);
        SDL_RenderDrawLine(renderer, x - 4 - 1, y + 4, x + 4 - 1, y - 4);
    }
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

//_________________________________________________ 14
//Get Screen Location
Point2D<int> Point3D::GetScreenLocation(Point2D<int> origin, int grid_size)
{
    Point2D<int> loc(0,0);
    loc.x = ScreenTransformX(_pos->x, origin, grid_size);
    loc.y = ScreenTransformY(_pos->y, origin, grid_size);

    return loc;
}

//_________________________________________________ 15
//SetLocation
void Point3D::SetLocation(double x, double y, double z)
{
    this->_pos->x = x;
    this->_pos->y = y;
    this->_pos->z = z;
}



//_________________________________________________ 1
//Line Constructor
Line::Line(double &&x1, double &&y1, double &&x2, double &&y2) : _x1(x1),
                                                                 _y1(y1),
                                                                 _z1(0.0),
                                                                 _x2(x2),
                                                                 _y2(y2),
                                                                 _z2(0.0)
{
}

Line::Line(const double &x1, const double &y1, const double &x2, const double &y2) : _x1(x1),
                                                                                     _y1(y1),
                                                                                     _z1(0.0),
                                                                                     _x2(x2),
                                                                                     _y2(y2),
                                                                                     _z2(0.0)
{
}

//copy constructor(deep copies)
Line::Line(const Line &line) : _x1(line._x1),
                               _y1(line._y1),
                               _z1(line._z1),
                               _x2(line._x2),
                               _y2(line._y2),
                               _z2(line._z2)
{
}
//assignment copy operator(deep copies)
Line &Line::operator=(const Line &line) 
{
    if (this == &line)
    {
        return *this;
    }
    else
    {
        this->_x1 = line._x1;
        this->_y1 = line._y1;
        this->_z1 = line._z1;
        _color = line._color;
        return *this;
    }
}

//move constructor(deep copies)
Line::Line(Line &&line) : _x1(line._x1),
                          _y1(line._y1),
                          _z1(line._z1),
                          _x2(line._x2),
                          _y2(line._y2),
                          _z2(line._z2)
{
}
//move assignment copy operator(deep copies)
Line &Line::operator=(Line &&line) 
{
    if (this == &line)
    {
        return *this;
    }
    else
    {
        this->_x1 = line._x1;
        this->_y1 = line._y1;
        this->_z1 = line._z1;
        _color = line._color;
        return *this;
    }
}



//_________________________________________________ 2
//Line Drawer
void Line::Draw(SDL_Renderer *renderer, Point2D<int> origin, int gridscale, int mode)
{
    if (mode == 1)
    {
        SDL_SetRenderDrawColor(renderer, _color.r, _color.g, _color.b, _color.a);
    }
    else
    {
        SDL_SetRenderDrawColor(renderer, selection_color_grayscale, selection_color_grayscale, selection_color_grayscale, _color.a);
    }

    int x1 = ScreenTransformX(_x1, origin, gridscale);
    int y1 = ScreenTransformY(_y1, origin, gridscale);
    int x2 = ScreenTransformX(_x2, origin, gridscale);
    int y2 = ScreenTransformY(_y2, origin, gridscale);

    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

//_________________________________________________ 1
//Curve Constructor
Curve::Curve(std::vector<std::shared_ptr<Point3D>>& points, int degree) : _points(points)
{
    if(degree < 3)
    {
        _degree = 1;
    }else
    {
        _degree = 3;
    }
}

void Curve::Draw(SDL_Renderer *renderer, Point2D<int> origin, int gridscale, int mode)
{
    if (mode == 1)
    {
        SDL_SetRenderDrawColor(renderer, _color.r, _color.g, _color.b, _color.a);
    }
    else
    {
        SDL_SetRenderDrawColor(renderer, selection_color_grayscale, selection_color_grayscale, selection_color_grayscale, _color.a);
    }

    std::vector<Point2D<double>> tempList;
    for (auto &p : _points)
    {
        double x = (double)ScreenTransformX(p->GetLocation().x, origin, gridscale);
        double y = (double)ScreenTransformY(p->GetLocation().y, origin, gridscale);
        tempList.emplace_back(Point2D<double>(x, y));
    }

    if (_degree == 3)
    {
        for (size_t j = 0; j < 3; ++j)
        {
            std::vector<Point2D<double>> tempList2;
            for (size_t i = 0; i < tempList.size() - 2; i++)
            {
                if (i == 0)
                {
                    Point2D<double> be1 = tempList.front();
                    tempList2.emplace_back(std::move(be1));
                }

                Point2D<double> oneQ;
                oneQ.x = ((((double)tempList[i].x) * 0.5f) + (((double)tempList[i + 1].x) * 0.5f));
                oneQ.y = ((((double)tempList[i].y) * 0.5f) + (((double)tempList[i + 1].y) * 0.5f));

                tempList2.emplace_back(std::move(oneQ));
                Point2D<double> threeQ;
                threeQ.x = (((double)tempList[i].x) * 0.125f + ((double)tempList[i + 1].x) * 0.75f + ((double)tempList[i + 2].x) * 0.125f);
                threeQ.y = (((double)tempList[i].y) * 0.125f + ((double)tempList[i + 1].y) * 0.75f + ((double)tempList[i + 2].y) * 0.125f);

                tempList2.emplace_back(std::move(threeQ));
                Point2D<double> fQ;
                fQ.x = ((((double)tempList[i + 1].x) * 0.5f) + (((double)tempList[i + 2].x) * 0.5f));
                fQ.y = ((((double)tempList[i + 1].y) * 0.5f) + (((double)tempList[i + 2].y) * 0.5f));
                tempList2.emplace_back(std::move(fQ));

                if (i == tempList.size() - 3)
                {
                    Point2D<double> be2 = tempList.back();
                    tempList2.emplace_back(std::move(be2));
                }
            }
            tempList = tempList2;
        }
    }

    for (size_t i = 0; i < tempList.size() - 1; ++i)
    {
        //Drawing the center curve
        SDL_RenderDrawLine(renderer, tempList[i].x, tempList[i].y, tempList[i + 1].x, tempList[i + 1].y);
    }
}