#pragma once
#include <vector>
#include "Triangle.h"
#include "pch.h"

namespace GeometricEntity {
    class GEOMETRY_API Triangulation
    {
    public:
        Triangulation(std::vector<Triangle> triangles);
        ~Triangulation();
        std::vector<Triangle> getTriangles() const;
    private:
        std::vector<Triangle> mTriangles;
    };
}