#pragma once
#ifndef POINT3D_H
#define POINT3D_H
#include "pch.h"

namespace GeometricEntity {

    class GEOMETRY_API Point3D
    {
    public:
        Point3D(float inX, float inY, float inZ);
        ~Point3D();

        float x() const;
        float y() const;
        float z() const;

        void setX(float x);
        void setY(float y);
        void setZ(float z);

    private:
        float mX;
        float mY;
        float mZ;
    };
}
#endif POINT3D_H

