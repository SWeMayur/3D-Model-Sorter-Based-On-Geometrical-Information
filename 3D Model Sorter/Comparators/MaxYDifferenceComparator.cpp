#include "pch.h"
#include <vector>
#include "MaxYDifferenceComparator.h"
#include "Point3D.h"
using namespace GeometricEntity;

bool MaxYDifferenceComparator::operator()(const std::vector<Point3D>& v1, const std::vector<Point3D>& v2) const {
    float maxYV1 = -12345678.0f;
    float maxYV2 = -12345678.0f;

    for (int i = 0; i < v1.size(); i++) {
        if (v1[i].y() > maxYV1) maxYV1 = v1[i].y();
    }

    for (int i = 0; i < v2.size(); i++) {
        if (v2[i].y() > maxYV2) maxYV2 = v2[i].y();
    }
    return maxYV1 > maxYV2;
}
