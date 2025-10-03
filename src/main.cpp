#include <iostream>
#include <vector>
#include <set>
#include "triangle.h"

int main()
{
    int N = 0;
    std::cin >> N;
    
    std::vector<Triangle> triangles(N);
    
    for (int i = 0; i < N; i++)
    {
        triangles[i].id = i;
        std::cin >> triangles[i].vertices[0].x >> triangles[i].vertices[0].y >> triangles[i].vertices[0].z;
        std::cin >> triangles[i].vertices[1].x >> triangles[i].vertices[1].y >> triangles[i].vertices[1].z;
        std::cin >> triangles[i].vertices[2].x >> triangles[i].vertices[2].y >> triangles[i].vertices[2].z;
        
        Point3D v1 = triangles[i].vertices[1] - triangles[i].vertices[0];
        Point3D v2 = triangles[i].vertices[2] - triangles[i].vertices[0];
        triangles[i].normal = normalize(cross(v1, v2));
    }
    
    std::set<int> intersectingTriangles;
    
    for (int i = 0; i < N; i++)
    {
        for (int j = i + 1; j < N; j++)
        {
            if (trianglesIntersect(triangles[i], triangles[j]))
            {
                intersectingTriangles.insert(i);
                intersectingTriangles.insert(j);
            }
        }
    }
    
    for (int id : intersectingTriangles)
    {
        std::cout << id << "\n";
    }
    
    return 0;
}
