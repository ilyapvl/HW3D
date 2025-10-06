#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <vector>

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
};

struct Triangle
{
    int id;
    Point3D vertices[3];
    
    Triangle(int id, const Point3D& v0, const Point3D& v1, const Point3D& v2);
};

double dot(const Point3D& a, const Point3D& b);
Point3D cross(const Point3D& a, const Point3D& b);
double tripleProduct(const Point3D& a, const Point3D& b, const Point3D& c);
bool pointInTriangle(const Point3D& p, const Point3D& t0, const Point3D& t1, const Point3D& t2);
void projectTriangle(const Triangle& t, const Point3D& axis, double& minProj, double& maxProj);
bool coplanarTrianglesIntersect(const Triangle& t1, const Triangle& t2);
bool trianglesIntersect(const Triangle& t1, const Triangle& t2);
std::vector<int> findIntersectingTriangles(const std::vector<Triangle>& triangles);


bool areCoplanar(const Triangle& t1, const Triangle& t2);

#endif



