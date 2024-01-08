#include "stdafx.h"
#include "OpenGLWindow.h"
#include <QOpenGLContext>
#include <QOpenGLPaintDevice>
#include <QOpenGLShaderProgram>
#include <QPainter>
#include <fstream>
#include <sstream>
#include <random>
#include "STLReader.h"
#include "Point3D.h"

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

    mProgram->bind();

    QMatrix4x4 matrix;
    matrix.rotate(rotationAngle);
    
    matrix.ortho(-30.0f, 430.0f, -130.0f, 430.0f, 0.1f, 100.0f);

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

        glDrawArrays(GL_LINE_LOOP, 0, mAllVertices[i].size() / 3);
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
    static const char* vertexShaderSource =
        "attribute highp vec4 posAttr;\n"
        "attribute lowp vec4 colAttr;\n"
        "varying lowp vec4 col;\n"
        "uniform highp mat4 matrix;\n"
        "void main() {\n"
        "   col = colAttr;\n"
        "   gl_Position = matrix * posAttr;\n"
        "}\n";

    static const char* fragmentShaderSource =
        "varying lowp vec4 col;\n"
        "void main() {\n"
        "   gl_FragColor = col;\n"
        "}\n";

    initializeOpenGLFunctions();
    setMouseTracking(true);

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
        // Handle the error appropriately, e.g., return or throw an exception
    }
}

void OpenGLWindow::readSTL(std::string &filePath)
{
    mVertices.clear();
    mColors.clear();
    STLReader reader(filePath, mVertices, mColors);
}

void OpenGLWindow::getAllFiles(QStringList &filePathList)
{
    for (int i = 0; i < filePathList.size(); i++)
    {   
        std::string filePath = filePathList[i].toStdString();
        readSTL(filePath);
        pq.push(mVertices);
    }
    storeSortedFiles();

}

void OpenGLWindow::storeSortedFiles()
{   
    int x = 1;
    while (!pq.empty())
    {
        std::vector<Point3D> v = pq.top();
        pq.pop();
        std::vector<float> c;
        for (Point3D p : v) {
            c.push_back(p.x());
            c.push_back(p.y());
            c.push_back(p.z());
        }
        mAllVertices.push_back(c);

        std::vector<float> colors;
        if (x == 1)
        {
            for (Point3D p : mColors) {
                colors.push_back(1.0f);
                colors.push_back(0.0f);
                colors.push_back(0.0f);
            }
        }
        else if (x == 2)
        {
            for (Point3D p : mColors) {
                colors.push_back(0.0f);
                colors.push_back(1.0f);
                colors.push_back(0.0f);
            }
        }
        else if(x == 3)
        {
            for (Point3D p : mColors) {
                colors.push_back(0.0f);
                colors.push_back(0.0f);
                colors.push_back(1.0f);
            }
        }
        else if(x == 4)
        {
            for (Point3D p : mColors) {
                colors.push_back(1.0f);
                colors.push_back(1.0f);
                colors.push_back(0.0f);
            }
        }
        else 
        {
            for (Point3D p : mColors) {
                colors.push_back(1.0f);
                colors.push_back(0.0f);
                colors.push_back(1.0f);
            }
        }
        mAllColors.push_back(colors);
        x++;
    }
}