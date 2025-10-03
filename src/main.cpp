#include "triangle.h"
#include <iostream>
#include <vector>

int main()
{
    int N = 0;
    std::cin >> N;
    
    std::vector<Triangle> triangles(N);
    
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
    std::cout << std::endl;
    
    return 0;
}
