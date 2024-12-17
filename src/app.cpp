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
    std::shared_ptr<Cube> cube = std::make_shared<Cube>(1.0f, glm::vec3(0.0f, 0.0f, 1.0f + i));

    std::shared_ptr<Material> material = std::make_shared<Material>();
    material->ambientColor = glm::vec3(0.1f, 0.1f, 0.1f);  // Тусклый амбиентный цвет
    material->diffuseColor = glm::vec3(0.6f, 0.6f, 0.6f);  // Тусклый диффузный цвет
    material->specularColor = glm::vec3(1.0f, 1.0f, 1.0f);  // Нет зеркальных отражений (матовый)
    material->shininess = 10.0f;  // Шершавость поверхности, меньше — более матовая

    // material->ambientColor = glm::vec3(0.1f, 0.1f, 0.1f);  // Тусклый амбиентный цвет
    // material->diffuseColor = glm::vec3(0.8f, 0.8f, 0.8f);  // Яркий диффузный цвет
    // material->specularColor = glm::vec3(1.0f, 1.0f, 1.0f);  // Полностью зеркальные отражения (металлический эффект)
    // material->shininess = 1.0f;  // Высокая шершавость (показывает более четкие отражения)

    // material->ambientColor = glm::vec3(0.1f, 0.1f, 0.1f);  // Тусклый амбиентный цвет
    // material->diffuseColor = glm::vec3(0.5f, 0.5f, 0.5f);  // Средний диффузный цвет
    // material->specularColor = glm::vec3(1.0f, 1.0f, 1.0f);  // Яркие зеркальные отражения
    // material->shininess = 50.0f;  // Средняя шершавость (для пластика)

    // material->ambientColor = glm::vec3(0.2f, 0.1f, 0.05f);  // Темный амбиентный цвет, под дерево
    // material->diffuseColor = glm::vec3(0.6f, 0.3f, 0.1f);  // Теплый диффузный цвет (деревянный оттенок)
    // material->specularColor = glm::vec3(0.2f, 0.1f, 0.05f);  // Слабые зеркальные отражения
    // material->shininess = 30.0f;  // Меньше шершавости, типично для деревянных поверхностей

    // material->ambientColor = glm::vec3(0.24725f, 0.1995f, 0.0745f);  // Тусклый амбиентный цвет, похожий на золото
    // material->diffuseColor = glm::vec3(0.75164f, 0.60648f, 0.22648f);  // Диффузный цвет золота
    // material->specularColor = glm::vec3(0.628281f, 0.555802f, 0.366065f);  // Зеркальные отражения для золота
    // material->shininess = 51.2f;  // Средняя шершавость, для блеска золота


    cube->setMaterial(material);
    openglWidget->addShape(cube);
    
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
        ObjLoader loader;
        std::vector<std::shared_ptr<Mesh>> meshes = loader.load(filePath.toStdString());
        for (const auto mesh: meshes) {
            openglWidget->addShape(mesh);
        }
        qDebug() << "Add" << meshes.size() << "meshes!";
    }
}

void MainWindow::addLightButtonClicked() 
{
    qDebug() << "Add light!";
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
