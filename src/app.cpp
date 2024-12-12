#include "app.h"
#include "./ui_mainwindow.h"
#include <QDebug>

namespace {
    const int WIDTH = 512;
    const int HEIGT = 512;
}

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) 
{
    ui->setupUi(this);
    connect(ui->addCube, &QPushButton::clicked, this, &MainWindow::addCubeButtonClicked);
    openglWidget = new OpenGLWidget(ui->openGLWidget); // Используем контейнер из .ui
    setWindowTitle("Игры Разума");
    setGeometry(400, 200, 800, 600);
    qDebug() << openglWidget->size();
    openglWidget->resize(720, 720);
}

void MainWindow::addCubeButtonClicked() 
{   
    openglWidget->addShape(std::make_shared<Cube>(1.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0, 0, 0.5)));
    i += 1;
}

MainWindow::~MainWindow() 
{
    delete ui;
}
