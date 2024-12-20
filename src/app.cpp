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
    connect(ui->changeProjection, &QPushButton::clicked, this, &MainWindow::changeProjectionButtonClicked);
    connect(ui->startScene, &QPushButton::clicked, this, &MainWindow::startScene);
    connect(ui->restorePosition, &QPushButton::clicked, this, &MainWindow::restorePosition);
    setWindowTitle("Игры Разума");
    setGeometry(400, 200, 1280, 720);

    openglWidget = new OpenGLWidget(ui->openGLWidget);
    openglWidget->resize(920, 512);

    fpsUpdateTimer = std::make_shared<QTimer>(this);
    connect(fpsUpdateTimer.get(), &QTimer::timeout, this, &MainWindow::displayFPS);
    fpsUpdateTimer->start(1000);  // Обновление FPS каждую секунду
}

void MainWindow::addCubeButtonClicked()
{
    std::shared_ptr<Cube> cube = std::make_shared<Cube>(1.0f, glm::vec3(0.0f, 0.0f, 1.0f + i));

    std::shared_ptr<Material> material = std::make_shared<Material>();
    material->ambientColor = glm::vec3(1, 1, 1);
    material->diffuseColor = glm::vec3(0.4f, 0.4f, 0.4f);
    material->specularColor = glm::vec3(0.7f, 0.7f, 0.7f);
    material->shininess = 10.0f;

    cube->setPosition(glm::vec3(0, 0, 0));
    cube->setRotation(glm::vec3(0, 0, 0));
    cube->setScale(glm::vec3(1, 1, 1));
    cube->setMaterial(material);
    openglWidget->addShape(cube);

    i += 1;
}

void MainWindow::displayFPS()
{
    ui->fps->display(openglWidget->getFPS());
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
            mesh->setPosition(glm::vec3(0, 0, 0));
            mesh->setRotation(glm::vec3(0, 0, 0));
            mesh->setScale(glm::vec3(1, 1, 1));
        }
        qDebug() << "Add" << meshes.size() << "meshes!";
    }
}

void MainWindow::changeProjectionButtonClicked()
{
    openglWidget->changeCameraProjection();
}

void MainWindow::startScene()
{
    openglWidget->startScene();
}

void MainWindow::restorePosition()
{
    openglWidget->restorePosition();
}

void MainWindow::addLightButtonClicked()
{
    qDebug() << "Add light!";
    std::shared_ptr<Light> pointLight = std::make_shared<PointLight>(
        glm::vec3(-3.0f, -3.0f, -3.0f)     // Позиция источника света
    );

    std::shared_ptr<Light> spotLight = std::make_shared<SpotLight>(
        glm::vec3(5.0f, 5.0f, 5.0f),       // Позиция прожектора
        glm::vec3(1.0f, 1.0f, 1.0f)      // Направление прожектора
    );

    std::shared_ptr<Light> dirLight = std::make_shared<DirectionalLight>(
        glm::vec3(-1.0f, -1.0f, -1.0f)      // Направление света (например, вниз)
    );
    openglWidget->addLight(pointLight);
    // openglWidget->addLight(spotLight);
    // openglWidget->addLight(spotLight);
    // openglWidget->addLight(spotLight);
}

MainWindow::~MainWindow()
{
    delete ui;
}
