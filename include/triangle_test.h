#ifndef TRIANGLE_TEST_H
#define TRIANGLE_TEST_H

#include "triangle.h"

void runAllTests();
void testPointInTriangle();
void testProjectTriangle();
void testCoplanarity();
void testCoplanarIntersection();
void testNonCoplanarIntersection();
void testEdgeCaseIntersections();
void testFindIntersectingTriangles();
bool equal(double a, double b);

#endif
