#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QMainWindow>
#include <QQuaternion>
#include <QWheelEvent>
#include <queue>
#include "Point3D.h"
#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <map>

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
    void getAllFiles(QStringList &filePathList);

public:
    std::map<std::string, float> costMap;

protected:
    void paintGL() override;
    void initializeGL() override;

private:
    void reset();
    void storeSortedFiles();
signals:
    void shapeUpdate();



private:
    bool mAnimating = false;
    QOpenGLContext* mContext = nullptr;
    QOpenGLPaintDevice* mDevice = nullptr;

    QOpenGLShader* mVshader = nullptr;
    QOpenGLShader* mFshader = nullptr;
    QOpenGLShaderProgram* mProgram = nullptr;
    

    //QList<QVector3D> mVertices;
    //QList<QVector3D> mNormals;
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

    std::vector<std::vector<float>> mAllVertices;
    std::vector<std::vector<float>> mAllColors;

    std::map<int, float> stlPriceMap;

    float zoomFactor = 1.0f;

    struct MaxYDifferenceComparator {
        bool operator()(const vector<Point3D>& v1, const vector<Point3D>& v2) const {
            // Calculate the maximum difference in y coordinates for each vector
            float maxYV1 = -12345678.0f;
            float maxYV2 = -12345678.0f;
            for (int i = 0; i < v1.size(); i++) {
                if (v1[i].y() > maxYV1) maxYV1 = v1[i].y();
            }
            for (int i = 0; i < v2.size(); i++) {
                if (v2[i].y() > maxYV2) maxYV2 = v2[i].y();
            }

            return maxYV1 < maxYV2;
        }
    };

    struct MaxPriceComparator {
        bool operator()(const vector<Point3D>& v1, const vector<Point3D>& v2) const {
            
        }
    };

    std::priority_queue<std::vector<Point3D>, std::vector<std::vector<Point3D>>, MaxYDifferenceComparator> pq;
};   



/*QFile file("Price.json");
            QString s1 = QString::number(v1.size());
            QString s2 = QString::number(v2.size());
            QByteArray jsonData = file.readAll();
            QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonData);

            QJsonObject jsonObject = jsonDocument.object();
            QJsonValue v11 = jsonObject.value(s1);
            QJsonValue v22 = jsonObject.value(s2);
            return v11.toInt() > v22.toInt();*/