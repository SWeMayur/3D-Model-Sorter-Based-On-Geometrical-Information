#include <iostream>
#include <sstream>
#include <fstream>
#include "pch.h"
#include "ModelGenerator.h"
#include "Point3D.h"
using namespace GeometricEntity;


ModelGenerator::ModelGenerator(std::string filePath)
{
    mFilePath = filePath;
}

ModelGenerator::~ModelGenerator()
{
}

void ModelGenerator::updateSTL(std::string outputFilePath, int scaleFactor)
{
    std::ifstream dataFile;
    dataFile.open(mFilePath);
    if (!dataFile.is_open())
    {
        return;
    }

    std::string line;
    std::ofstream writeFile;
    writeFile.open(outputFilePath);
    Point3D normal(0, 0, 0);
    while (std::getline(dataFile, line))
    {
        if (line.find("vertex") != std::string::npos)
        {

            std::istringstream currentLine(line);
            std::string token;
            double x, y, z;
            currentLine >> token >> x >> y >> z;
            writeFile << "  vertex" << " " << x * 10 << " " << y * 10 << " " << z * 10 << std::endl;

            std::getline(dataFile, line);
            std::istringstream currentLine1(line);
            currentLine1 >> token >> x >> y >> z;
            writeFile << "  vertex" << " " << x * 10 << " " << y * 10 << " " << z * 10 << std::endl;

            std::getline(dataFile, line);
            std::istringstream currentLine2(line);
            currentLine2 >> token >> x >> y >> z;
            writeFile << "  vertex" << " " << x * 10 << " " << y * 10 << " " << z * 10 << std::endl;
        }
        else writeFile << line << std::endl;
    }
    dataFile.close();
}

