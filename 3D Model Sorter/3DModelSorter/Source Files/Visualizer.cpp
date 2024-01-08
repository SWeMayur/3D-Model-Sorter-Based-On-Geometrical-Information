#include "stdafx.h"
#include "Visualizer.h"
#include "OpenGLWindow.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
using namespace std;

Visualizer::Visualizer(QWindow* parent) : QMainWindow(nullptr)
{
    setupUi();
    connect(addFileButton, &QPushButton::clicked, this, &Visualizer::openFileSelectionDialog);
    connect(doneFileSelect, &QPushButton::clicked, this, &Visualizer::completeFileSelect);
    connect(sortButton, &QPushButton::clicked, this, &Visualizer::callSort);
}

Visualizer::~Visualizer()
{}

void Visualizer::setupUi() 
{
    resize(1870, 970);
    addFileButton = new QPushButton(this);
    addFileButton->setObjectName("mpushButton");
    addFileButton->setText("Add File");
    addFileButton->setGeometry(QRect(1640, 45, 100, 25));
    
    doneFileSelect = new QPushButton(this);
    doneFileSelect->setObjectName("mpushButton");
    doneFileSelect->setText("Done");
    doneFileSelect->setGeometry(QRect(1640, 400, 100, 25));

    listOfFiles = new QListView(this);
    listOfFiles->setGeometry(QRect(1640, 125, 150, 250));

    fileModel = new QStringListModel(this);
    listOfFiles->setModel(fileModel);
    
    chooseParameterLabel = new QLabel("Parameter",this);
    chooseParameterLabel->setGeometry(QRect(1640, 500, 150, 25));

    parameterList = new QComboBox(this);
    parameterList->setGeometry(QRect(1640, 525, 150, 25));
    parameterList->addItem("Height");
    parameterList->addItem("Cost");
    parameterList->addItem("Year of Estabishment");
    parameterList->addItem("Metal Use");

    sortButton = new QPushButton(this);
    sortButton->setObjectName("mpushButton");
    sortButton->setText("Sort");
    sortButton->setGeometry(QRect(1640, 700, 150, 25));

    // Position the menu below the listOfFiles
    mRenderer = new OpenGLWindow(QColor(0, 0, 0), this);
    mRenderer->setGeometry(QRect(100, 30, 1500, 900));

    setWindowTitle(QCoreApplication::translate("3D Model Sorter", "3D Model Sorter", nullptr));
}

void Visualizer::openFileSelectionDialog() 
{

        QString filePath = QFileDialog::getOpenFileName(this, tr("Open STL File"), "", tr("STL Files (*.stl);;All Files (*)"));
        QFileInfo fileInfo(filePath);
        fileList.append(fileInfo.fileName());
        filePathList.append(filePath);
}

void Visualizer::completeFileSelect()
{
    fileModel->setStringList(fileList);
}

void Visualizer::callSort()
{
    mRenderer->getAllFiles(filePathList);;
}
