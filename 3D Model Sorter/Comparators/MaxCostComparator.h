#pragma once
#include <vector>
#include <string>
#include <map>
#include "Point3D.h"
using namespace GeometricEntity;

class COMPARATORS_API MaxCostComparator {

public:
    MaxCostComparator();
    ~MaxCostComparator();

    bool operator()(const std::vector<Point3D>& v1, const std::vector<Point3D>& v2) const;

private:
    void readJsonFile(const std::string& jsonFilePath);

private:
    std::map<float, int> jsonValues;
};

