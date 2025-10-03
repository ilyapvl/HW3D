#include "triangle.h"
#include <vector>
#include <cmath>
#include <set>



Point3D::Point3D() : x(0), y(0), z(0){}

Point3D::Point3D(double x, double y, double z) : x(x), y(y), z(z){}

bool Point3D::operator==(const Point3D& other) const
{
    return std::abs(x - other.x) < EPS && 
           std::abs(y - other.y) < EPS && 
           std::abs(z - other.z) < EPS;
}

Point3D Point3D::operator+(const Point3D& other) const
{
    return Point3D(x + other.x, y + other.y, z + other.z);
}

Point3D Point3D::operator-(const Point3D& other) const
{
    return Point3D(x - other.x, y - other.y, z - other.z);
}

Point3D Point3D::operator*(double scalar) const
{
    return Point3D(x * scalar, y * scalar, z * scalar);
}



Triangle::Triangle(int id, const Point3D& v0, const Point3D& v1, const Point3D& v2) : id(id)
{
    vertices[0] = v0;
    vertices[1] = v1;
    vertices[2] = v2;
}

double dot(const Point3D& a, const Point3D& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Point3D cross(const Point3D& a, const Point3D& b)
{
    return Point3D(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

double tripleProduct(const Point3D& a, const Point3D& b, const Point3D& c)
{
    return dot(a, cross(b, c));
}

void projectTriangle(const Triangle& t, const Point3D& axis, double& minProj, double& maxProj)
{
    minProj = maxProj = dot(t.vertices[0], axis);
    for (int i = 1; i < 3; i++)
    {
        double proj = dot(t.vertices[i], axis);
        minProj = std::min(minProj, proj);
        maxProj = std::max(maxProj, proj);
    }
}

bool pointInTriangle(const Point3D& p, const Point3D& t0, const Point3D& t1, const Point3D& t2)
{
    Point3D v0 = t1 - t0;
    Point3D v1 = t2 - t0;
    Point3D v2 = p - t0;
    
    double d00 = dot(v0, v0);
    double d01 = dot(v0, v1);
    double d11 = dot(v1, v1);
    double d20 = dot(v2, v0);
    double d21 = dot(v2, v1);
    
    double denom = d00 * d11 - d01 * d01;
    if (std::abs(denom) < EPS)
    {
        return false;
    }
    
    double v = (d11 * d20 - d01 * d21) / denom;
    double w = (d00 * d21 - d01 * d20) / denom;
    double u = 1.0 - v - w;
    
    return (u >= -EPS) && (v >= -EPS) && (w >= -EPS);
}

bool coplanarTrianglesIntersect(const Triangle& t1, const Triangle& t2)
{
    for (int i = 0; i < 3; i++)
    {
        if (pointInTriangle(t1.vertices[i], t2.vertices[0], t2.vertices[1], t2.vertices[2]))
        {
            return true;
        }
    }
    
    for (int i = 0; i < 3; i++)
    {
        if (pointInTriangle(t2.vertices[i], t1.vertices[0], t1.vertices[1], t1.vertices[2]))
        {
            return true;
        }
    }
    
    for (int i = 0; i < 3; i++)
    {
        Point3D p1 = t1.vertices[i];
        Point3D p2 = t1.vertices[(i + 1) % 3];
        
        for (int j = 0; j < 3; j++)
        {
            Point3D q1 = t2.vertices[j];
            Point3D q2 = t2.vertices[(j + 1) % 3];
            
            Point3D dir1 = p2 - p1;
            Point3D dir2 = q2 - q1;
            
            Point3D normal = cross(dir1, dir2);
            if (std::abs(normal.x) < EPS && std::abs(normal.y) < EPS && std::abs(normal.z) < EPS)
            {
                continue;
            }
            
            Point3D w = p1 - q1;
            double d = dot(cross(dir1, dir2), cross(dir1, dir2));
            if (std::abs(d) < EPS) continue;
            
            double s = dot(cross(dir2, w), cross(dir1, dir2)) / d;
            double t = dot(cross(dir1, w), cross(dir1, dir2)) / d;
            
            if (s >= -EPS && s <= 1.0 + EPS && t >= -EPS && t <= 1.0 + EPS)
            {
                return true;
            }
        }
    }
    
    return false;
}

bool trianglesIntersect(const Triangle& t1, const Triangle& t2)
{
    Point3D normal1 = cross(t1.vertices[1] - t1.vertices[0], t1.vertices[2] - t1.vertices[0]);
    Point3D normal2 = cross(t2.vertices[1] - t2.vertices[0], t2.vertices[2] - t2.vertices[0]);
    
    double vol1 = tripleProduct(normal1, t2.vertices[0] - t1.vertices[0], normal2);
    if (std::abs(vol1) < EPS)
    {
        return coplanarTrianglesIntersect(t1, t2);
    }
    
    Point3D D = cross(normal1, normal2);
    
    std::vector<Point3D> axes;
    axes.push_back(normal1);
    axes.push_back(normal2);
    
    for (int i = 0; i < 3; i++)
    {
        Point3D edge1 = t1.vertices[(i + 1) % 3] - t1.vertices[i];
        for (int j = 0; j < 3; j++)
        {
            Point3D edge2 = t2.vertices[(j + 1) % 3] - t2.vertices[j];
            Point3D testAxis = cross(edge1, edge2);
            
            if (dot(testAxis, testAxis) < EPS)
            {
                continue;
            }
            
            axes.push_back(testAxis);
        }
    }
    
    for (const Point3D& axis : axes)
    {
        double min1, max1, min2, max2;
        projectTriangle(t1, axis, min1, max1);
        projectTriangle(t2, axis, min2, max2);
        
        if (max1 < min2 - EPS || max2 < min1 - EPS)
        {
            return false;
        }
    }
    
    return true;
}

std::vector<int> findIntersectingTriangles(const std::vector<Triangle>& triangles)
{
    std::set<int> result;
    int n = triangles.size();
    
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (trianglesIntersect(triangles[i], triangles[j]))
            {
                result.insert(i);
                result.insert(j);
            }
        }
    }
    
    return std::vector<int>(result.begin(), result.end());
}
