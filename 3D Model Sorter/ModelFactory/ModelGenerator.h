#pragma once
#include <vector>
#include <string>
#include "Triangle.h"
#include "pch.h"

class ModelGenerator {
public:
    ModelGenerator(std::string filePath);
    ~ModelGenerator();

    void updateSTL(std::string filePath, int scaleFactor);

private:
    std::string mFilePath;
};