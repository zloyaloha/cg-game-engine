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
    connect(ui->addLight, &QPushButton::clicked, this, &MainWindow::addLightButtonClicked);
    connect(ui->addMesh, &QPushButton::clicked, this, &MainWindow::addMeshButtonClicked);
    setWindowTitle("Игры Разума");
    setGeometry(400, 200, 1280, 720);

    openglWidget = new OpenGLWidget(ui->openGLWidget);
    openglWidget->resize(920, 512);
}

void MainWindow::addCubeButtonClicked() 
{   
    openglWidget->addShape(std::make_shared<Cube>(1.0f, glm::vec3(0.0f, 0.0f, 1.0f + i), glm::vec3(1.0, 0, 0.5)));
    i += 1;
}

void MainWindow::addMeshButtonClicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Select File", "", "Text Files (*.obj);;All Files (*)");

    if (filePath.isEmpty()) {
        return;
    }

    if (QFile::exists(filePath)) {
        qDebug() << "Load mesh from " << filePath;
        std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(glm::vec3(0.0f, 0.0f, 1.0f + i), glm::vec3(1.0, 1.0, 1.0));
        mesh->loadFromFile(filePath.toStdString());
        openglWidget->addShape(mesh);
        i += 1;
    }
}

void MainWindow::addLightButtonClicked() 
{
    std::shared_ptr<Light> light = std::make_shared<Light>();
    light->color = glm::vec3(1.0, 1.0, 1.0);
    light->position = glm::vec3(10.0, 10.0, 10.0 + i);
    light->pointTo = glm::vec3(0.0, 0.0, 0.0);
    openglWidget->addLight(light);
}

MainWindow::~MainWindow() 
{
    delete ui;
}
