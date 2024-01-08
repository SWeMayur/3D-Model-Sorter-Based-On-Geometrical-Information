#include "pch.h"
#include "Point3D.h"
using namespace GeometricEntity;

Point3D::Point3D(float inX, float inY, float inZ)
    :mX(0),
    mY(0),
    mZ(0)
{
    mX = inX;
    mY = inY;
    mZ = inZ;
}

Point3D::~Point3D()
{
}

float Point3D::x() const
{
    return mX;
}

float Point3D::y() const
{
    return mY;
}

float Point3D::z() const
{
    return mZ;
}

void Point3D::setX(float x) {
    mX = x;
}

void Point3D::setY(float y) {
    mY = y;
}

void Point3D::setZ(float z) {
    mZ = z;
}


