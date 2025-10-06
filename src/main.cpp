#include "triangle.h"
#include <iostream>
#include <vector>
#include <string>
#include "triangle_test.h"

int main(int argc, char** argv)
{
    if (argc > 1)
    {
        if (std::strcmp(argv[1], "--t") == 0)
        {
            runAllTests();

            return 0;
        }
    }

    int N = 0;
    std::cin >> N;
    
    std::vector<Triangle> triangles;
    
    for (int i = 0; i < N; i++)
    {
        double x1, y1, z1, x2, y2, z2, x3, y3, z3;
        std::cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> x3 >> y3 >> z3;
        
        triangles.emplace_back(i, Point3D(x1, y1, z1), Point3D(x2, y2, z2), Point3D(x3, y3, z3));
    }
    
    std::vector<int> intersecting = findIntersectingTriangles(triangles);
    
    for (int id : intersecting)
    {
        std::cout << id << "\n";
    }
    
    return 0;
}
