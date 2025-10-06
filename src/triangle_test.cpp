#include "triangle_test.h"
#include <cassert>
#include <iostream>
#include <vector>
#include <cmath>

void runAllTests()
{
    std::cout << "=== TRIANGLE TESTS ===" << std::endl;
    
    testPointInTriangle();
    testProjectTriangle();
    testCoplanarity();
    testCoplanarIntersection();
    testNonCoplanarIntersection();
    testEdgeCaseIntersections();
    testFindIntersectingTriangles();
    
    std::cout << "=== ALL TESTS PASSED ===" << std::endl;
}

void testPointInTriangle()
{
    std::cout << "Testing point in triangle" << std::endl;
    Point3D t0(0, 0, 0);
    Point3D t1(1, 0, 0);
    Point3D t2(0, 1, 0);
    
    // Edge cases
    assert(pointInTriangle(Point3D(0.001, 0.001, 0), t0, t1, t2));
    assert(pointInTriangle(Point3D(0.999, 0.001, 0), t0, t1, t2));
    assert(pointInTriangle(Point3D(0.001, 0.999, 0), t0, t1, t2));
    
    assert(!pointInTriangle(Point3D(-0.001, 0.5, 0), t0, t1, t2));
    assert(!pointInTriangle(Point3D(0.5, -0.001, 0), t0, t1, t2));
    assert(!pointInTriangle(Point3D(0.501, 0.501, 0), t0, t1, t2));
    
    assert(pointInTriangle(Point3D(0.999, 0.0001, 0), t0, t1, t2));
    assert(pointInTriangle(Point3D(0.0001, 0.999, 0), t0, t1, t2));
}

void testProjectTriangle()
{
    std::cout << "Testing triangle projection" << std::endl;
    
    // Very small triangle
    Triangle small_t(0, Point3D(0, 0, 0), Point3D(0.001, 0, 0), Point3D(0, 0.001, 0));
    
    double minProj, maxProj;
    projectTriangle(small_t, Point3D(1, 0, 0), minProj, maxProj);
    assert(equal(minProj, 0) && equal(maxProj, 0.001));
    
    projectTriangle(small_t, Point3D(0, 1, 0), minProj, maxProj);
    assert(equal(minProj, 0) && equal(maxProj, 0.001));
    
    // Very small axis
    projectTriangle(small_t, Point3D(0.001, 0.001, 0), minProj, maxProj);
    assert(equal(minProj, 0) && equal(maxProj, 0.000002));
}

void testCoplanarity()
{
    std::cout << "Testing coplanarity detection" << std::endl;
    
    // Almost coplanar
    Triangle t1(0, Point3D(0, 0, 0), Point3D(1, 0, 0), Point3D(0, 1, 0));
    Triangle t2(1, Point3D(0, 0, 0.001), Point3D(1, 0, 0.001), Point3D(0, 1, 0.001));
    assert(trianglesIntersect(t1, t2) == false);
    
    Triangle t3(2, Point3D(0, 0, 0.0001), Point3D(1, 0, 0.0001), Point3D(0, 1, 0.0001));
    assert(trianglesIntersect(t1, t3) == false);
    
    // Almost coplanar but less than EPS
    Triangle t4(3, Point3D(0, 0, EPS/2), Point3D(1, 0, EPS/2), Point3D(0, 1, EPS/2));
    assert(trianglesIntersect(t1, t4) == true);
}

void testCoplanarIntersection()
{
    std::cout << "Testing coplanar triangle intersection" << std::endl;
    
    Triangle t1(0, Point3D(0, 0, 0), Point3D(1, 0, 0), Point3D(0, 1, 0));
    
    // Very small intersection
    Triangle t2(1, Point3D(0.999, 0.001, 0), Point3D(1.001, 0.001, 0), Point3D(0.999, 1.001, 0));
    assert(coplanarTrianglesIntersect(t1, t2) == true);
    
    // Very small touching
    Triangle t3(2, Point3D(1.001, 0, 0), Point3D(1.002, 0, 0), Point3D(1.001, 0.001, 0));
    assert(coplanarTrianglesIntersect(t1, t3) == false);
    
    // Almost inside
    Triangle t4(3, Point3D(0.001, 0.001, 0), Point3D(0.999, 0.001, 0), Point3D(0.001, 0.999, 0));
    assert(coplanarTrianglesIntersect(t1, t4) == true);
    
    // Almost intersecting but no
    Triangle t5(4, Point3D(1.001, 1.001, 0), Point3D(1.002, 1.001, 0), Point3D(1.001, 1.002, 0));
    assert(coplanarTrianglesIntersect(t1, t5) == false);
}

void testNonCoplanarIntersection()
{
    std::cout << "Testing non-coplanar triangle intersection" << std::endl;
    
    Triangle t1(0, Point3D(0, 0, 0), Point3D(1, 0, 0), Point3D(0, 1, 0));
    
    // Almost touching
    Triangle t2(1, Point3D(0.5, 0.5, 0.001), Point3D(1.5, 0.5, 0.001), Point3D(0.5, 1.5, 0.001));
    assert(trianglesIntersect(t1, t2) == false);
    
    // Very small intersection
    Triangle t3(2, Point3D(0.5, 0.5, -0.001), Point3D(1.5, 0.5, 0.001), Point3D(0.5, 1.5, 0.001));
    assert(trianglesIntersect(t1, t3) == true);
    
    Triangle t4(3, Point3D(0.001, 0.001, 0.001), Point3D(0.002, 0.001, -0.001), Point3D(0.001, 0.002, -0.001));
    assert(trianglesIntersect(t1, t4) == true);
}

void testEdgeCaseIntersections()
{
    std::cout << "Testing edge case intersections" << std::endl;
    
    // Almost the same
    Triangle t1(0, Point3D(0, 0, 0), Point3D(1, 0, 0), Point3D(0, 1, 0));
    Triangle t2(1, Point3D(0.0001, 0.0001, 0.0001), Point3D(1.0001, 0.0001, 0.0001), Point3D(0.0001, 1.0001, 0.0001));
    assert(trianglesIntersect(t1, t2) == true);
    
    // Small triangle
    Triangle small(2, Point3D(0.1, 0.1, 0), Point3D(0.1001, 0.1, 0), Point3D(0.1, 0.1001, 0));
    assert(trianglesIntersect(t1, small) == true);
    
    // Almost degenerated
    Triangle degenerate(3, Point3D(0.5, 0.5, 0), Point3D(0.5001, 0.5001, 0), Point3D(0.5002, 0.5002, 0));
    //TODO - check of degenerated triangles
    
    // Almost parallel
    Triangle t3(4, Point3D(0, 0, 0.001), Point3D(1, 0, 0.001), Point3D(0, 1, 0.001));
    Triangle t4(5, Point3D(0, 0, 0.002), Point3D(1, 0, 0.002), Point3D(0, 1, 0.002));
    assert(trianglesIntersect(t3, t4) == false);
}

void testFindIntersectingTriangles()
{
    std::cout << "Testing find intersecting triangles" << std::endl;
    
    std::vector<Triangle> triangles;
    
    // Different types
    triangles.push_back(Triangle(0, Point3D(0, 0, 0), Point3D(1, 0, 0), Point3D(0, 1, 0)));
    triangles.push_back(Triangle(1, Point3D(0.999, 0.001, 0.001), Point3D(1.999, 0.001, 0.001), Point3D(0.999, 1.001, 0.001)));
    
    triangles.push_back(Triangle(2, Point3D(2.001, 0, 0), Point3D(3.001, 0, 0), Point3D(2.001, 1, 0)));
    triangles.push_back(Triangle(3, Point3D(2.002, 0.001, 0), Point3D(3.002, 0.001, 0), Point3D(2.002, 1.001, 0)));
    
    triangles.push_back(Triangle(4, Point3D(0.5, 0.5, -0.001), Point3D(1.5, 0.5, 0.001), Point3D(0.5, 1.5, 0.001)));
    triangles.push_back(Triangle(5, Point3D(0.6, 0.6, -0.002), Point3D(1.6, 0.6, 0), Point3D(0.6, 1.6, 0)));
    
    std::vector<int> result = findIntersectingTriangles(triangles);
    
    // 0 4 5 intersects
    assert(result.size() == 3);
    assert(std::find(result.begin(), result.end(), 0) != result.end());
    assert(std::find(result.begin(), result.end(), 4) != result.end());
    assert(std::find(result.begin(), result.end(), 5) != result.end());
    
    // 1 2 3 do not
    assert(std::find(result.begin(), result.end(), 1) == result.end());
    assert(std::find(result.begin(), result.end(), 2) == result.end());
    assert(std::find(result.begin(), result.end(), 3) == result.end());
}

bool equal(double a, double b)
{
    return std::abs(a - b) < EPS;
}
