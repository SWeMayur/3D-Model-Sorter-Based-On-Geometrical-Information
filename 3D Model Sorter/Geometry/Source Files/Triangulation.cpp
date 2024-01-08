#include "pch.h"
#include "Triangulation.h"
using namespace GeometricEntity;

Triangulation::Triangulation(std::vector<Triangle> triangles)
{
    mTriangles = triangles;
}

Triangulation::~Triangulation()
{
}

std::vector<Triangle> Triangulation::getTriangles() const
{
    return mTriangles;
}
