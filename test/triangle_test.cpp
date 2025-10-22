#include "triangle.h"
#include <gtest/gtest.h>
#include <vector>
#include <cmath>

using namespace Geometry3D;
using namespace testing;

// Vector Math Tests
TEST(VectorMathTest, DotProduct) {
    Point3D a(1, 2, 3);
    Point3D b(4, 5, 6);
    EXPECT_DOUBLE_EQ(dot(a, b), 32.0);
}

TEST(VectorMathTest, CrossProduct) {
    Point3D a(1, 0, 0);
    Point3D b(0, 1, 0);
    Point3D result = cross(a, b);
    EXPECT_DOUBLE_EQ(result.x, 0.0);
    EXPECT_DOUBLE_EQ(result.y, 0.0);
    EXPECT_DOUBLE_EQ(result.z, 1.0);
}

TEST(VectorMathTest, TripleProduct) {
    Point3D a(1, 0, 0);
    Point3D b(0, 1, 0);
    Point3D c(0, 0, 1);
    EXPECT_DOUBLE_EQ(tripleProduct(a, b, c), 1.0);
}

// Individual triangle tests

TEST(TriangleTest, PointInTriangle) {
    Triangle t1(0, Point3D(0,0,0), Point3D(1,0,0), Point3D(0,1,0));
    Point3D inside(0.2, 0.2, 0);
    Point3D outside(1.0, 1.0, 0);
    
    EXPECT_TRUE(pointInTriangle(inside, t1.vertices[0], t1.vertices[1], t1.vertices[2]));
    EXPECT_FALSE(pointInTriangle(outside, t1.vertices[0], t1.vertices[1], t1.vertices[2]));
}

TEST(TriangleTest, ProjectTriangle) {
    Triangle t1(0, Point3D(0,0,0), Point3D(1,0,0), Point3D(0,1,0));
    Point3D axis(1, 0, 0);
    double minProj, maxProj;
    
    projectTriangle(t1, axis, minProj, maxProj);
    EXPECT_DOUBLE_EQ(minProj, 0.0);
    EXPECT_DOUBLE_EQ(maxProj, 1.0);
}

TEST(TriangleTest, CoplanarityDetection) {
    Triangle t1(0, Point3D(0, 0, 0), Point3D(1, 0, 0), Point3D(0, 1, 0));
    Triangle t2(1, Point3D(2, 0, 0), Point3D(3, 0, 0), Point3D(2, 1, 0));
    Triangle t3(2, Point3D(0, 0, 1), Point3D(1, 0, 1), Point3D(0, 1, 1));
    
    EXPECT_TRUE(areCoplanar(t1, t2));
    EXPECT_FALSE(areCoplanar(t1, t3)); 
}

TEST(TriangleTest, NonIntersectingTriangles) {
    Triangle t1(0, Point3D(0, 0, 0), Point3D(1, 0, 0), Point3D(0, 1, 0));
    Triangle t2(1, Point3D(2, 0, 0), Point3D(3, 0, 0), Point3D(2, 1, 0));
    
    EXPECT_FALSE(trianglesIntersect(t1, t2));
}

TEST(TriangleTest, IntersectingTriangles) {
    Triangle t1(0, Point3D(0, 0, 0), Point3D(2, 0, 0), Point3D(0, 2, 0));
    Triangle t2(1, Point3D(1, 0, 0), Point3D(1, 2, 0), Point3D(0, 1, 0));
    
    EXPECT_TRUE(trianglesIntersect(t1, t2));
}

TEST(TriangleTest, CoplanarIntersectingTriangles) {
    Triangle t1(0, Point3D(0, 0, 0), Point3D(1, 0, 0), Point3D(0, 1, 0));
    Triangle t2(1, Point3D(0.5, 0, 0), Point3D(1.5, 0, 0), Point3D(0.5, 1, 0));
    
    EXPECT_TRUE(trianglesIntersect(t1, t2));
}

TEST(EdgeCaseTest, DegenerateTriangle) {
    Point3D p1(0, 0, 0), p2(1, 0, 0), p3(2, 0, 0);
    Triangle degenerate(0, p1, p2, p3);
    Triangle normal(1, Point3D(0, 1, 0), Point3D(1, 1, 0), Point3D(0, 2, 0));
    
    EXPECT_NO_THROW(trianglesIntersect(degenerate, normal));
}

TEST(EdgeCaseTest, IdenticalTriangles) {
    Triangle t1(0, Point3D(0, 0, 0), Point3D(1, 0, 0), Point3D(0, 1, 0));
    Triangle t2(1, Point3D(0, 0, 0), Point3D(1, 0, 0), Point3D(0, 1, 0));
    
    EXPECT_TRUE(trianglesIntersect(t1, t2));
}

TEST(EdgeCaseTest, TouchingTriangles) {
    Triangle t1(0, Point3D(0, 0, 0), Point3D(1, 0, 0), Point3D(0, 1, 0));
    Triangle t2(1, Point3D(0, 0, 0), Point3D(0, -1, 0), Point3D(-1, 0, 0));
    
    EXPECT_TRUE(trianglesIntersect(t1, t2));
}




// More triangle operations
TEST(TriangleTest, SmallTriangleProjection) {
    Triangle t(0, 
        Point3D(0.001, 0.002, 0.003), 
        Point3D(0.004, 0.005, 0.006), 
        Point3D(0.007, 0.008, 0.009)
    );
    Point3D axis(1.0, 0.0, 0.0);
    double minProj, maxProj;
    
    projectTriangle(t, axis, minProj, maxProj);
    EXPECT_NEAR(minProj, 0.001, EPS);
    EXPECT_NEAR(maxProj, 0.007, EPS);
}

TEST(TriangleTest, SmallTrianglePointInTriangle) {
    Point3D t0(0.0, 0.0, 0.0);
    Point3D t1(0.01, 0.0, 0.0);
    Point3D t2(0.0, 0.01, 0.0);
    
    Point3D inside(0.002, 0.002, 0.0);
    Point3D outside(0.008, 0.008, 0.0);
    
    EXPECT_TRUE(pointInTriangle(inside, t0, t1, t2));
    EXPECT_FALSE(pointInTriangle(outside, t0, t1, t2));
}

TEST(TriangleTest, SmallCoplanarTriangles) {
    Triangle t1(0, 
        Point3D(0.0, 0.0, 0.0), 
        Point3D(0.01, 0.0, 0.0), 
        Point3D(0.0, 0.01, 0.0)
    );
    Triangle t2(1, 
        Point3D(0.005, 0.0, 0.0), 
        Point3D(0.015, 0.0, 0.0), 
        Point3D(0.005, 0.01, 0.0)
    );
    
    EXPECT_TRUE(areCoplanar(t1, t2));
    EXPECT_TRUE(trianglesIntersect(t1, t2));
}

TEST(TriangleTest, SmallNonIntersectingTriangles) {
    Triangle t1(0, 
        Point3D(0.0, 0.0, 0.0), 
        Point3D(0.01, 0.0, 0.0), 
        Point3D(0.0, 0.01, 0.0)
    );
    Triangle t2(1, 
        Point3D(0.02, 0.0, 0.0), 
        Point3D(0.03, 0.0, 0.0), 
        Point3D(0.02, 0.01, 0.0)
    );
    
    EXPECT_FALSE(trianglesIntersect(t1, t2));
}

TEST(TriangleTest, SmallTouchingTriangles) {
    Triangle t1(0, 
        Point3D(0.0, 0.0, 0.0), 
        Point3D(0.01, 0.0, 0.0), 
        Point3D(0.0, 0.01, 0.0)
    );
    Triangle t2(1, 
        Point3D(0.0, 0.0, 0.0), 
        Point3D(-0.01, 0.0, 0.0), 
        Point3D(0.0, -0.01, 0.0)
    );
    
    EXPECT_TRUE(trianglesIntersect(t1, t2));
}

// Edge case with very small triangle near origin
TEST(TriangleTest, VerySmallTriangle) {
    Triangle t(0, 
        Point3D(0.0001, 0.0002, 0.0003), 
        Point3D(0.0002, 0.0001, 0.0003), 
        Point3D(0.00001, 0.0001, 0.0004)
    );
    
    Point3D axis(1.0, 1.0, 1.0);
    double minProj, maxProj;
    
    projectTriangle(t, axis, minProj, maxProj);
    // Just verify it doesn't crash and produces reasonable values
    EXPECT_LT(minProj, maxProj);
}

// End-to-End Tests
TEST(EndToEndTest, NoIntersections) {
    std::vector<Triangle> triangles = {
        Triangle(0, Point3D(0, 0, 0), Point3D(1, 0, 0), Point3D(0, 1, 0)),
        Triangle(1, Point3D(2, 0, 0), Point3D(3, 0, 0), Point3D(2, 1, 0)),
        Triangle(2, Point3D(4, 0, 0), Point3D(5, 0, 0), Point3D(4, 1, 0))
    };
    
    std::vector<int> result = findIntersectingTriangles(triangles);
    EXPECT_TRUE(result.empty());
}

TEST(EndToEndTest, MultipleIntersections) {
    std::vector<Triangle> triangles = {
        Triangle(0, Point3D(0, 0, 0), Point3D(2, 0, 0), Point3D(0, 2, 0)),
        Triangle(1, Point3D(1, 0, 0), Point3D(3, 0, 0), Point3D(1, 2, 0)),
        Triangle(2, Point3D(0.5, 0, 0), Point3D(2.5, 0, 0), Point3D(0.5, 2, 0))
    };
    
    std::vector<int> result = findIntersectingTriangles(triangles);
    EXPECT_EQ(result.size(), 3);
}

TEST(EndToEndTest, SingleIntersectionPair) {
    std::vector<Triangle> triangles = {
        Triangle(0, Point3D(0, 0, 0), Point3D(1, 0, 0), Point3D(0, 1, 0)),
        Triangle(1, Point3D(2, 0, 0), Point3D(3, 0, 0), Point3D(2, 1, 0)),
        Triangle(2, Point3D(0.5, 0, 0), Point3D(1.5, 0, 0), Point3D(0.5, 1, 0)) 
    };
    
    std::vector<int> result = findIntersectingTriangles(triangles);
    EXPECT_EQ(result.size(), 2);
}

TEST(PerformanceTest, ManyNonIntersectingTriangles) {
    std::vector<Triangle> triangles;
    int n = 1000;
    /* 

    Time complexity of findIntersectingtriangles() seems to be O(n^2)
    but I can't figure out better geometrical approach
    
    */
    
    for (int i = 0; i < n; ++i) {
        triangles.emplace_back(i, 
            Point3D(i * 3, 0, 0), 
            Point3D(i * 3 + 1, 0, 0), 
            Point3D(i * 3, 1, 0));
    }
    
    EXPECT_NO_THROW(findIntersectingTriangles(triangles));
}

int main(int argc, char **argv) {
    InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
