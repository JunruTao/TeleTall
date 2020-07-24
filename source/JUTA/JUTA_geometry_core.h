#ifndef JUTA_GEOMETRY_CORE_H
#define JUTA_GEOMETRY_CORE_H
#include "x_sdl2headers.h"
#include <memory>
#include <cmath>
#include <string>
#include <vector>
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

    void operator*=(var amplitue)
    {
        this->x *= amplitue;
        this->y *= amplitue;
    }

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

    double DistanceTo(const Point2D &vec2)
    {
        double len = sqrt(pow((double)vec2.x - (double)this->x, 2) + pow((double)vec2.y -(double)this->y, 2));
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

    bool InBoundWH(const var &minx, const var &miny, const var &width, const var &height)
    {
        return (this->x > minx) && (this->x < minx+width) && (this->y > miny) && (this->y < miny+height);
    }

    void New(const var &_x, const var &_y)
    {
        this->x = _x;
        this->y = _y;
    }

    Point2D GetUnitVector()
    {
        Point2D unitv;
        unitv.x = this->x / this->Length();
        unitv.y = this->y / this->Length();
        return unitv;
    }

    void MakeUnitVector()
    {
        this->x = (var)(this->x/this->Length());
        this->y = (var)(this->y/this->Length());
    }
};






class GeoData
{
public:
    GeoData() { _color = {255, 0, 0, 255}; };
    virtual ~GeoData(){};
    virtual void Draw(SDL_Renderer *renderer, Point2D<int> origin, int gridscale, int mode) = 0;

protected:
    SDL_Color _color;
    static int selection_color_grayscale;
};





struct Vector3
{
    Vector3(double ix, double iy, double iz) : x(ix), y(iy), z(iz){};
    double x;
    double y;
    double z;
};





class Point3D : public GeoData
{
public:
    //Rule of 5 
    //constructors
    Point3D();
    Point3D(double x, double y, double z);
    Point3D(const Vector3& pos);
    Point3D(Vector3&& pos);

    //copy constructor(deep copies)
    Point3D(const Point3D& point);
    //assignment copy operator(deep copies)
    Point3D &operator=(const Point3D& point);

    //move constructor(deep copies)
    Point3D(Point3D&& point);
    //move assignment copy operator(deep copies)
    Point3D &operator=(Point3D&& point);

    //Destructor
    ~Point3D();
    //Override functions
    void Draw(SDL_Renderer *renderer, Point2D<int> origin, int gridscale, int mode) override;
 
    void Translate(const double& x,const double& y,const double& z);
    void Translate(const Vector3& motion);
    void Translate(Vector3&& motion);

private:
    std::unique_ptr<Vector3> _pos;
};





class Line : public GeoData
{
    public:
    Line(std::shared_ptr<Point3D> a, std::shared_ptr<Point3D> b);
    ~Line();

    void Draw(SDL_Renderer *renderer, Point2D<int> origin, int gridscale, int mode) override;

    private:
    std::shared_ptr<Point3D> A;
    std::shared_ptr<Point3D> B;
};



class Polyline: public GeoData
{
    public:
    Polyline(std::vector<std::shared_ptr<Point3D>>& points);
    ~Polyline();

    void Draw(SDL_Renderer *renderer, Point2D<int> origin, int gridscale, int mode) override;

    private:
    std::vector<std::shared_ptr<Point3D>> points;
};
#endif