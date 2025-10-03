#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <cmath>

const double EPS = 1e-12;

struct Point3D
{
    double x, y, z;
    
    Point3D();
    Point3D(double x, double y, double z);
    
    Point3D operator+(const Point3D& other) const;
    Point3D operator-(const Point3D& other) const;
    Point3D operator*(double scalar) const;
    
    bool operator==(const Point3D& other) const;
    bool operator<(const Point3D& other) const;
};

struct Triangle
{
    int id;
    Point3D vertices[3];
    Point3D normal;
};

Point3D cross(const Point3D& a, const Point3D& b);
double dot(const Point3D& a, const Point3D& b);
double length(const Point3D& v);
Point3D normalize(const Point3D& v);
bool equals(double a, double b);
bool pointOnSegment(const Point3D& p, const Point3D& a, const Point3D& b);
bool pointOnTriangle(const Point3D& p, const Triangle& tri);
bool pointInsideTriangle(const Point3D& p, const Triangle& tri);

bool segmentsIntersect(const Point3D& a, const Point3D& b, const Point3D& c, const Point3D& d);
bool trianglesIntersect(const Triangle& t1, const Triangle& t2);


#endif
