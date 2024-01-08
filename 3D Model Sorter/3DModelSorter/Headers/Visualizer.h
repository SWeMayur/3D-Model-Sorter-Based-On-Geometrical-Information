#pragma once

#include <QtWidgets/QMainWindow>
#include "OpenGLWindow.h"
#include <Qvector>

class OpenGLWindow;

class Visualizer : public QMainWindow {
    Q_OBJECT

public:
    Visualizer(QWindow* parent = nullptr);
    ~Visualizer();

private:
    void setupUi();
    void openFileSelectionDialog();
    void completeFileSelect();
    void callSort();

private:
    QWidget* mCentralWidget;
    OpenGLWindow* mRenderer;

    QPushButton* doneFileSelect;
    QPushButton* addFileButton;
    QPushButton* sortButton;

    QStringList fileList;
    QStringList filePathList;

    QStringListModel* fileModel;

    QVector<GLfloat>mVertices;
    QVector<GLfloat>mColors;

    QListView* listOfFiles;
    QComboBox* parameterList;
    QLabel* chooseParameterLabel;

};
