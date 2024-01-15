#include "stdafx.h"
#include "OpenGLWindow.h"
#include <QOpenGLContext>
#include <QOpenGLPaintDevice>
#include <QOpenGLShaderProgram>
#include <QPainter>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <random>
#include <cstdlib>
#include "STLReader.h"
#include "Point3D.h"
#include <iostream>
#include "JsonFileReader.h"

using namespace IOOperation;
using namespace GeometricEntity;

OpenGLWindow::OpenGLWindow(const QColor& background, QMainWindow* parent) :
    mBackground(background) {
    setParent(parent);
    setMinimumSize(300, 250);
}

OpenGLWindow::~OpenGLWindow() {
    reset();
}

void OpenGLWindow::reset() {
    makeCurrent();
    delete mProgram;
    mProgram = nullptr;
    delete mVshader;
    mVshader = nullptr;
    delete mFshader;
    mFshader = nullptr;
    mVbo.destroy();
    doneCurrent();

    QObject::disconnect(mContextWatchConnection);
}

void OpenGLWindow::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(135.0f / 255.0f, 206.0f / 255.0f, 235.0f / 255.0f, 1.0f);
    mProgram->bind();

    QMatrix4x4 matrix;
    matrix.rotate(rotationAngle);
    matrix.ortho(-30.0f, 430, -130.0f, 430.0f, -130.0f, 100.0f);
    matrix.scale(zoomFactor);
    for (int i = 0; i < mAllVertices.size(); i++)
    {
        matrix.translate(30, 0, 0);
        matrix.rotate(180.0f, 0.0f, 1.0f, 0.0f);
        mProgram->setUniformValue(m_matrixUniform, matrix);
        GLfloat* verticesData = mAllVertices[i].data();
        GLfloat* colorsData = mAllColors[i].data();

        glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, verticesData);
        glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colorsData);

        glEnableVertexAttribArray(m_posAttr);
        glEnableVertexAttribArray(m_colAttr);

        glDrawArrays(GL_TRIANGLES, 0, mAllVertices[i].size() / 3);
        matrix.translate(-30, 0, 0);
        matrix.rotate(-180.0f, 0.0f, 1.0f, 0.0f);
        mProgram->setUniformValue(m_matrixUniform, matrix);
    }
}

void OpenGLWindow::mouseMoveEvent(QMouseEvent* event) {
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        QQuaternion rotX = QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, 0.5f * dx);
        QQuaternion rotY = QQuaternion::fromAxisAndAngle(1.0f, 0.0f, 0.0f, 0.5f * dy);

        rotationAngle = rotX * rotY * rotationAngle;
        update();
    }
    lastPos = event->pos();
}

void OpenGLWindow::initializeGL() {

    initializeOpenGLFunctions();
    setMouseTracking(true);
    QString vertexShaderSource = readShader("vertexShader.glsl");
    QString fragmentShaderSource = readShader("fragmentShader.glsl");

    mProgram = new QOpenGLShaderProgram(this);
    mProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    mProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    mProgram->link();
    m_posAttr = mProgram->attributeLocation("posAttr");

    m_colAttr = mProgram->attributeLocation("colAttr");

    m_matrixUniform = mProgram->uniformLocation("matrix");

    if (m_posAttr == -1 || m_colAttr == -1 || m_matrixUniform == -1)
    {
        qDebug() << "Shader attribute or uniform location error.";
    }
}

QString OpenGLWindow::readShader(QString filepath)
{
    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return "Invalid file";
    QTextStream stream(&file);
    QString line = stream.readLine();
    while (!stream.atEnd())
    {
        line.append(stream.readLine());
    }
    return line;
}

void OpenGLWindow::wheelEvent(QWheelEvent* event)
{
    int delta = event->angleDelta().y();
    // Adjust the zoom factor based on the mouse wheel movement
    if (delta > 0) {
        // Zoom in
        zoomFactor *= 1.1f;
    }
    else {
        // Zoom out
        zoomFactor /= 1.1f;
    }
    update();
}


void OpenGLWindow::readSTL(std::string &filePath)
{
    mVertices.clear();
    mColors.clear();
    STLReader reader(filePath, mVertices, mColors, mNormals);
    QFileInfo fileName(QString::fromStdString(filePath));
}


void OpenGLWindow::getAllFiles(QStringList &mFilePathList)
{
    for (int i = 0; i < mFilePathList.size(); i++)
    {   
        std::string filePath = mFilePathList[i].toStdString();
        readSTL(filePath);
        sortedSTLList.push_back(mVertices);
    }
    storeSortedFiles();
}

void OpenGLWindow::writeVerticesMapToJson(std::string &filePath, std::unordered_map<std::string, int>& json_data)
{
    JsonFileReader reader(filePath, json_data);
}

void OpenGLWindow::selectSortMethod()
{
    switch (choice) 
    {
    case 1:
    {
        MaxYDifferenceComparator heightComparator;
        std::sort(sortedSTLList.begin(), sortedSTLList.end(), heightComparator);
        break;
    }
    case 2:
    {
        MaxCostComparator costComparator;
        std::sort(sortedSTLList.begin(), sortedSTLList.end(), costComparator);
        break;
    }
    case 3:
    {
        YearOfEstablishComparator ageComparator;
        std::sort(sortedSTLList.begin(), sortedSTLList.end(), ageComparator);
        break;
    }
    default: break;
    }
}

void OpenGLWindow::storeSortedFiles()
{   
    mAllVertices.clear();
    mAllColors.clear();
    selectSortMethod();

    for(int i = 0 ; i < sortedSTLList.size() ; i ++)
    {
        std::vector<Point3D> v = sortedSTLList[i];
        std::vector<float> c;
        for (Point3D p : v) {

            c.push_back(p.x());
            c.push_back(p.y());
            c.push_back(p.z());
        }
       
        mAllVertices.push_back(c);
        std::vector<float> colors;
        for (Point3D p : mColors) {
            colors.push_back(p.x());
            colors.push_back(p.y());
            colors.push_back(p.z());
        }
        mAllColors.push_back(colors);
    }
    sortedSTLList.clear();
    update();
}


void OpenGLWindow::setTableContent(QStringList& mFileList, QTableWidget& mModelTable)
{
    for (int i = 0; i < mFileList.size(); i++)
    {
        // Insert a new row
        mModelTable.insertRow(i);

        // Set Model Name in column 0
        QTableWidgetItem* modelNameItem = new QTableWidgetItem(mFileList[i]);
        modelNameItem->setTextAlignment(Qt::AlignCenter);
        mModelTable.setItem(i, 0, modelNameItem);

        // Set Height in column 1
        std::unordered_map<std::string, int> map;
        std::string str = "Height.json";
        writeVerticesMapToJson(str, map);
        int key = map[mFileList[i].toStdString()];
        QString val = QString::number(key) + " m";
        QTableWidgetItem* heightItem = new QTableWidgetItem(val);
        heightItem->setTextAlignment(Qt::AlignCenter);
        mModelTable.setItem(i, 1, heightItem);

        // Set Price in column 2
        str = "Price.json";
        writeVerticesMapToJson(str, map);
        key = map[mFileList[i].toStdString()];
        val = QString::number(key);
        QTableWidgetItem* priceItem = new QTableWidgetItem(val);
        priceItem->setTextAlignment(Qt::AlignCenter);
        mModelTable.setItem(i, 2, priceItem);

        // Set Year in column 3
        str = "Year.json";
        writeVerticesMapToJson(str, map);
        key = map[mFileList[i].toStdString()];
        val = QString::number(key);
        QTableWidgetItem* yearItem = new QTableWidgetItem(val);
        yearItem->setTextAlignment(Qt::AlignCenter);
        mModelTable.setItem(i, 3, yearItem);
    }

  
}
