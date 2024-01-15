#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <qopenglcontext.h>
#include <QMainWindow>
#include <QQuaternion>
#include <QWheelEvent>
#include <queue>
#include <vector>
#include <algorithm>
#include <map>
#include "MaxYDifferenceComparator.h"
#include "YearOfEstablishComparator.h"
#include "MaxCostComparator.h"
#include "Point3D.h"

using namespace std;
using namespace GeometricEntity;

class QOpenGLTexture;
class QOpenGLShader;
class QOpenGLShaderProgram;
class QOpenGLPaintDevice;

class OpenGLWindow : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT
public:
    OpenGLWindow(const QColor& background, QMainWindow* parent);
    ~OpenGLWindow();
    void mouseMoveEvent(QMouseEvent* event);
    void readSTL(std::string &filePath);
    void getAllFiles(QStringList &mFilePathList);
    void setTableContent(QStringList& mFileList, QTableWidget& mModelTable);

public:
    std::map<std::string, float> costMap;
    int choice = -1;

protected:
    void paintGL() override;
    void initializeGL() override;

private:
    void reset();
    void storeSortedFiles();
    void writeVerticesMapToJson(std::string& filePath, std::unordered_map<std::string, int> &map);
    QString readShader(QString filepath);
    void wheelEvent(QWheelEvent* event);
    void selectSortMethod();
signals:
    void shapeUpdate();

private:
    bool mAnimating = false;
    QOpenGLContext* mContext = nullptr;
    QOpenGLPaintDevice* mDevice = nullptr;

    QOpenGLShader* mVshader = nullptr;
    QOpenGLShader* mFshader = nullptr;
    QOpenGLShaderProgram* mProgram = nullptr;

    QOpenGLBuffer mVbo;
    double mVertexAttr;
    double mNormalAttr;
    double mMatrixUniform;
    QColor mBackground;
    QMetaObject::Connection mContextWatchConnection;

    GLint m_posAttr = 0;
    GLint m_colAttr = 0;
    GLint m_matrixUniform = 0;

    QQuaternion rotationAngle;
    QPoint lastPos;

    std::vector<Point3D> mVertices;
    std::vector<Point3D> mColors;
    std::vector<Point3D> mNormals;

    std::vector<std::vector<float>> mAllVertices;
    std::vector<std::vector<float>> mAllColors;

    std::vector<float> mTotalVertices;
    std::vector<float> mTotalColors;

    std::map<float, int> verticesMap;

    std::vector<vector<Point3D>> sortedSTLList;

    float zoomFactor = 1.0f;
};   
