#include "triangle_test.h"
#include <cassert>
#include <iostream>
#include <vector>
#include <cmath>
#include <sstream>

void runAllTests()
{
    std::cout << "Triangle tests\n" << std::endl;
    
    testPointInTriangle();
    testProjectTriangle();
    testCoplanarity();
    testCoplanarIntersection();
    testNonCoplanarIntersection();

    std::cout << "Unit tests passed" << std::endl;

    endToEndTest();
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
    
    // Almost touching but no
    Triangle t4(3, Point3D(0.001, 0.001, 0.001), Point3D(0.002, 0.001, -0.001), Point3D(0.001, 0.002, -0.001));
    assert(trianglesIntersect(t1, t4) == true);
}

bool equal(double a, double b)
{
    return std::abs(a - b) < EPS;
}

void endToEndTest()
{   
    std::cout << "\nEnd-to-end testing" << '\n';

    struct TestCase
    {
        std::string name;
        std::string input;
        std::string expected;
    };
    
    std::vector<TestCase> tests = {
        {
            "Coplanar triangles",
            "2\n"
            "0 0 0 2 0 0 0 2 0\n"
            "1 1 0 3 1 0 1 3 0",
            "0 1"
        },
        
        {
            "Non-intersecting", 
            "2\n"
            "0 0 0 1 0 0 0 1 0\n"
            "2 2 1 3 2 1 2 3 1",
            ""
        },
        
        {
            "One inside another",
            "2\n"
            "0 0 0 3 0 0 0 3 0\n"
            "1 1 0 2 1 0 1 2 0",
            "0 1"
        },
        
        {
            "Touching at vertex",
            "2\n"
            "0 0 0 1 0 0 0 1 0\n"
            "0 1 0 -1 1 0 0 2 0", 
            "0 1"
        },
        
        {
            "Touching at edge",
            "2\n"
            "0 0 0 2 0 0 0 2 0\n"
            "0 2 0 2 2 0 0 4 0",
            "0 1"
        },
        
        {
            "Intersection in 3D space",
            "2\n"
            "0 0 0 2 0 0 0 2 0\n"
            "1 0 -1 1 2 1 2 1 1",
            "0 1"
        },
        
        {
            "Almost parallel but no intersection",
            "2\n"
            "0 0 0 1 0 0 0 1 0\n"
            "0 0 0.001 1 0 0.001 0 1 0.001",
            ""
        },
        
        {
            "Three intersections",
            "3\n"
            "0 0 0 2 0 0 0 2 0\n"
            "1 1 0 3 1 0 1 3 0\n"
            "2 2 0 4 2 0 2 4 0",
            "0 1 2"
        },
        
        {
            "Small intersection",
            "2\n"
            "0 0 0 0.1 0 0 0 0.1 0\n"
            "0.005 0.005 0 0.015 0.005 0 0.005 0.015 0",
            "0 1"
        },
        
        {
            "2 of 3",
            "3\n"
            "0 0 0 1 0 0 0 1 0\n"
            "0.5 0.5 0 1.5 0.5 0 0.5 1.5 0\n"
            "2 2 0 3 2 0 2 3 0",
            "0 1"
        },

        {
            "Almost coplanar",
            "2\n"
            "0 0 0 1 0 0 0 1 0\n"
            "0 0 0.01 5 5 0.01 5 5 0",
            ""
        },

        {
            "Small gap",
            "2\n"
            "0 0 0 1 0 0 0 1 0\n"
            "1.001 0 0 2.001 0 0 1.001 1 0",
            ""
        },

    };
    
    int passed = 0;
    int total = tests.size();
    
    for (const auto& test : tests)
    {
        std::istringstream input_stream(test.input);
        std::streambuf* old_cin = std::cin.rdbuf(input_stream.rdbuf());
        
        std::ostringstream output_stream;
        std::streambuf* old_cout = std::cout.rdbuf(output_stream.rdbuf());
        
        int n = 0;
        std::cin >> n;
        
        std::vector<Triangle> triangles;
        
        for (int i = 0; i < n; i++)
        {
            double x1, y1, z1, x2, y2, z2, x3, y3, z3;
            std::cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> x3 >> y3 >> z3;
            
            triangles.emplace_back(i, Point3D(x1, y1, z1), Point3D(x2, y2, z2), Point3D(x3, y3, z3));
        }
        
        std::vector<int> intersecting = findIntersectingTriangles(triangles);
        
        std::ostringstream result_stream;
        if (!intersecting.empty())
        {
            for (size_t i = 0; i < intersecting.size(); i++)
            {
                if (i > 0) result_stream << " ";
                result_stream << intersecting[i];
            }
        }
        std::string result = result_stream.str();
        
        std::cin.rdbuf(old_cin);
        std::cout.rdbuf(old_cout);
        
        if (result == test.expected)
        {
            passed++;
        }
        else
        {
            std::cout << "Failed test " << test.name << std::endl;
            std::cout << "Expected: " << test.expected << std::endl;
            std::cout << "Got: " << result << std::endl;
        }
    }

    std::cout << "Passed: " << passed << "/" << total << std::endl;
}
