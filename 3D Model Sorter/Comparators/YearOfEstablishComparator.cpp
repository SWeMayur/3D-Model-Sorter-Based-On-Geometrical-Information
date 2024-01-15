#include "pch.h"
#include "YearOfEstablishComparator.h"
#include <fstream>
#include <sstream>
#include "Point3D.h"
#include "string"
#include "algorithm"
using namespace GeometricEntity;

YearOfEstablishComparator::YearOfEstablishComparator() {
    const std::string& jsonFilePath = "Establishment.json";
    readJsonFile(jsonFilePath);
}

YearOfEstablishComparator::~YearOfEstablishComparator()
{}
bool YearOfEstablishComparator::operator()(const std::vector<Point3D>& v1, const std::vector<Point3D>& v2) const {
    float z1 = (v1.size() > 0) ? round(v1[0].z()) : 0.0f;
    float z2 = (v2.size() > 0) ? round(v2[0].z()) : 0.0f;

    int value1 = jsonValues.find(z1) != jsonValues.end() ? jsonValues.at(z1) : 0;
    int value2 = jsonValues.find(z2) != jsonValues.end() ? jsonValues.at(z2) : 0;

    return value1 < value2;
}

void YearOfEstablishComparator::readJsonFile(const std::string& jsonFilePath) {
    std::ifstream file(jsonFilePath);

    if (!file.is_open()) {
        throw std::runtime_error("Error opening file: " + jsonFilePath);
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string keyStr;
        int value;
        if (iss >> keyStr >> value) {
            float key = std::stof(keyStr.substr(1, 6));
            jsonValues[round(key)] = value;
        }
    }
}