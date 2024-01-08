#include "pch.h"
#include "Triangle.h"
using namespace GeometricEntity;


Triangle::Triangle(Point3D p1, Point3D p2, Point3D p3) :mP1(0, 0, 0), mP2(0, 0, 0), mP3(0, 0, 0)
{
    mP1 = p1;
    mP2 = p2;
    mP3 = p3;
}

Triangle::~Triangle()
{
}

Point3D Triangle::p1() const
{
    return mP1;
}

Point3D Triangle::p2() const
{
    return mP2;
}

Point3D Triangle::p3() const
{
    return mP3;
}
