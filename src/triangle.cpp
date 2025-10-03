#include "triangle.h"

Point3D::Point3D() : x(0), y(0), z(0){}

Point3D::Point3D(double x, double y, double z) : x(x), y(y), z(z){}

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

bool Point3D::operator==(const Point3D& other) const
{
    return std::fabs(x - other.x) < EPS && 
           std::fabs(y - other.y) < EPS && 
           std::fabs(z - other.z) < EPS;
}

bool Point3D::operator<(const Point3D& other) const
{
    if (std::fabs(x - other.x) > EPS) return x < other.x;
    if (std::fabs(y - other.y) > EPS) return y < other.y;
    return z < other.z;
}

Point3D cross(const Point3D& a, const Point3D& b)
{
    return Point3D(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

double dot(const Point3D& a, const Point3D& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

double length(const Point3D& v)
{
    return std::sqrt(dot(v, v));
}

Point3D normalize(const Point3D& v)
{
    double len = length(v);
    if (len < EPS) return v;
    return Point3D(v.x / len, v.y / len, v.z / len);
}

bool equals(double a, double b)
{
    return std::fabs(a - b) < EPS;
}

bool pointOnSegment(const Point3D& p, const Point3D& a, const Point3D& b)
{
    Point3D ab = b - a;
    Point3D ap = p - a;
    
    Point3D cross_product = cross(ab, ap);
    if (!equals(cross_product.x, 0) || !equals(cross_product.y, 0) || !equals(cross_product.z, 0))
    {
        return false;
    }
    
    double dot_product = dot(ap, ab);
    if (dot_product < -EPS) return false;
    
    double squared_length = dot(ab, ab);
    if (dot_product > squared_length + EPS) return false;
    
    return true;
}

bool pointOnTriangle(const Point3D& p, const Triangle& tri)
{
    for (int i = 0; i < 3; i++)
    {
        int j = (i + 1) % 3;
        if (pointOnSegment(p, tri.vertices[i], tri.vertices[j]))
        {
            return true;
        }
    }
    return false;
}

//NOTE - is it nessesary?
bool pointInsideTriangle(const Point3D& p, const Triangle& tri)
{
    Point3D v0 = tri.vertices[1] - tri.vertices[0];
    Point3D v1 = tri.vertices[2] - tri.vertices[0];
    Point3D v2 = p - tri.vertices[0];
    
    double d00 = dot(v0, v0);
    double d01 = dot(v0, v1);
    double d11 = dot(v1, v1);
    double d20 = dot(v2, v0);
    double d21 = dot(v2, v1);
    
    double denom = d00 * d11 - d01 * d01;
    if (equals(denom, 0)) return false;
    
    double v = (d11 * d20 - d01 * d21) / denom;
    double w = (d00 * d21 - d01 * d20) / denom;
    double u = 1.0 - v - w;
    
    return (u >= -EPS) && (v >= -EPS) && (w >= -EPS);
}

bool segmentsIntersect(const Point3D& a, const Point3D& b, const Point3D& c, const Point3D& d)
{
    Point3D ab = b - a;
    Point3D cd = d - c;
    Point3D ac = c - a;
    
    Point3D ab_cd = cross(ab, cd);
    double denom = dot(ab_cd, ab_cd);
    
    if (equals(denom, 0))
    {
        if (pointOnSegment(a, c, d) || pointOnSegment(b, c, d) || 
            pointOnSegment(c, a, b) || pointOnSegment(d, a, b))
        {
            return true;
        }
        return false;
    }
    
    double t = dot(cross(ac, cd), ab_cd) / denom;
    double u = dot(cross(ac, ab), ab_cd) / denom;
    
    if (t >= -EPS && t <= 1.0 + EPS && u >= -EPS && u <= 1.0 + EPS)
    {
        return true;
    }
    
    return false;
}

bool trianglesIntersect(const Triangle& t1, const Triangle& t2)
{
    for (int i = 0; i < 3; i++)
    {
        int j = (i + 1) % 3;
        for (int k = 0; k < 3; k++)
        {
            int l = (k + 1) % 3;
            if (segmentsIntersect(t1.vertices[i], t1.vertices[j], 
                                 t2.vertices[k], t2.vertices[l]))
            {
                return true;
            }
        }
    }
    
    for (int i = 0; i < 3; i++)
    {
        if (pointOnTriangle(t1.vertices[i], t2) || pointInsideTriangle(t1.vertices[i], t2))
        {
            return true;
        }
        if (pointOnTriangle(t2.vertices[i], t1) || pointInsideTriangle(t2.vertices[i], t1))
        {
            return true;
        }
    }
    
    return false;
}
