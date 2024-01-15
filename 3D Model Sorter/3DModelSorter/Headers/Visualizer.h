#pragma once

#include <QtWidgets/QMainWindow>
#include "OpenGLWindow.h"
#include <Qvector>
#include <set>

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
    void showModel();
    void handleComboBoxChange();

private:
    QWidget* mCentralWidget;
    OpenGLWindow* mRenderer;

    QPushButton* mDoneFileSelectButton;
    QPushButton* mAddFileButton;
    QPushButton* mSortButton;
    QPushButton* mShowModelButton;

    QStringList mFileList;
    QStringList mFilePathList;

    QVector<GLfloat>mVertices;
    QVector<GLfloat>mColors;

    QListView* mListOfFiles;
    QComboBox* mParameterList;
    QLabel* mChooseParameterLabel;

    QTableWidget* mModelTable;

    QHBoxLayout* mMainLayout;
    QSplitter* mSplitter;
    QVBoxLayout* mVerticalLayout;
};
