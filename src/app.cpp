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

    connect(ui->listWidget, &QListWidget::itemClicked, this, &MainWindow::onObjectSelected);
}

void MainWindow::addCubeButtonClicked() 
<<<<<<< HEAD
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

    cube->setPosition(glm::vec3(0, 0, 0));
    cube->setRotation(glm::vec3(0, 0, 0));
    cube->setScale(glm::vec3(1, 1, 1));
    cube->setMaterial(material);
    openglWidget->addShape(cube);
    
=======
{    
    addItemToList(i, "cube");
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 1.0f + i);
    glm::vec3 color = glm::vec3(1.0, 0, 0.5);
    openglWidget->addShape(std::make_shared<Cube>(1.0f, position, color));
>>>>>>> 87205a6 (added objects settings in the list)
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
<<<<<<< HEAD
            mesh->setPosition(glm::vec3(0, 0, 0));
            mesh->setRotation(glm::vec3(0, 0, 0));
            mesh->setScale(glm::vec3(1, 1, 1));
=======
            addItemToList(i, "mesh");
>>>>>>> 87205a6 (added objects settings in the list)
        }
        qDebug() << "Add" << meshes.size() << "meshes!";
    }
    
}

void MainWindow::addLightButtonClicked() 
{
    qDebug() << "Add light!";
    std::shared_ptr<Light> pointLight = std::make_shared<PointLight>(
        glm::vec3(-10.0f, -10.0f, -10.0f)     // Позиция источника света
    );

    std::shared_ptr<Light> spotLight = std::make_shared<SpotLight>(
        glm::vec3(5.0f, 5.0f, 5.0f),       // Позиция прожектора
        glm::vec3(1.0f, 1.0f, 1.0f)      // Направление прожектора
    );

    std::shared_ptr<Light> dirLight = std::make_shared<DirectionalLight>(
        glm::vec3(0.0f, -1.0f, 0.0f)      // Направление света (например, вниз)
    );
    openglWidget->addLight(spotLight);
    // openglWidget->addLight(spotLight);
    // openglWidget->addLight(dirLight);
}

void MainWindow::addItemToList(int i, const std::string& type)
{
    // Создаем новый элемент списка
    QListWidgetItem* newItem = new QListWidgetItem(QString::fromStdString(type), ui->listWidget);
    
    // Привязываем числовое значение к элементу
    newItem->setData(Qt::UserRole, i);  // Qt::UserRole можно использовать для хранения целых значений

    // Можно также привязать тип (строку) как дополнительные данные
    newItem->setData(Qt::UserRole + 1, QString::fromStdString(type));

    // Добавляем элемент в список
    ui->listWidget->addItem(newItem);
}

void MainWindow::onObjectSelected(QListWidgetItem *item)
{
    // Извлекаем числовое значение (например, ID)
    int id = item->data(Qt::UserRole).toInt();

    // Извлекаем строку с типом
    QString type = item->data(Qt::UserRole + 1).toString();

    // Выводим данные для проверки
    qDebug() << "Selected Item ID: " << id;
    qDebug() << "Item Type: " << type;

    // Например, если у вас есть указатель на объект Shape, привязанный к этим данным
    // Получаем объект Shape, например, из списка или другой структуры данных
    std::shared_ptr<Shape> selectedShape;
    selectedShape = openglWidget->getAllShapes()[id];
    if (selectedShape) {
        showObjectSettings(selectedShape);
    }
}


void MainWindow::showObjectSettings(std::shared_ptr<Shape> shape)
{
    QDialog settingsDialog;
    settingsDialog.setWindowTitle("Настройки объекта");
    settingsDialog.setMinimumSize(300, 200);

    // // Поля для редактирования позиции
    QLabel *positionLabel = new QLabel("Позиция (x, y, z):", &settingsDialog);
    glm::vec3 position =  shape->getPosition();
    glm::vec3 color = shape->getColor();    
    QLineEdit *positionX = new QLineEdit(QString::number(position.x), &settingsDialog);
    QLineEdit *positionY = new QLineEdit(QString::number(position.y), &settingsDialog);
    QLineEdit *positionZ = new QLineEdit(QString::number(position.z), &settingsDialog);
    // if (shape->getType() == "cube") {
    //     shape->setPosition(glm::vec3(positionX->text().toFloat(), positionY->text().toFloat(), positionZ->text().toFloat()));
    // }

    // Кнопка сохранения
    QPushButton *saveButton = new QPushButton("Сохранить", &settingsDialog);
    connect(saveButton, &QPushButton::clicked, [&]() {
        // Применяем изменения
        shape->setPosition(glm::vec3(positionX->text().toFloat(), positionY->text().toFloat(), positionZ->text().toFloat()));
        // dynamic_cast<Cube*>(shape)->setSize(scaleInput->text().toFloat());

        // Закрываем окно
        settingsDialog.accept();
    });

    // // Компоновка
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(positionLabel);
    layout->addWidget(positionX);
    layout->addWidget(positionY);
    layout->addWidget(positionZ);
    // layout->addWidget(scaleLabel);
    // layout->addWidget(scaleInput);
    // layout->addWidget(saveButton);

    settingsDialog.setLayout(layout);
    settingsDialog.exec();
}


MainWindow::~MainWindow() 
{
    delete ui;
}
