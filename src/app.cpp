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
    connect(ui->listWidget, &QListWidget::itemClicked, this, &MainWindow::onObjectSelected);
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
    std::string name = "cube" + std::to_string(i);
    addItemToList(i, name);
    openglWidget->addShape(cube);
    i += 1;
}

std::string MainWindow::getPenultimateWord(const QString& qstringPath) {
    // Преобразуем QString в std::string
    std::string path = qstringPath.toStdString();

    // Разделяем строку по "/"
    std::vector<std::string> parts;
    std::stringstream ss(path);
    std::string segment;
    while (std::getline(ss, segment, '/')) {
        parts.push_back(segment);
    }

    // Проверяем, есть ли достаточно частей
    if (parts.size() < 2) {
        return ""; // Возвращаем пустую строку, если частей недостаточно
    }

    // Возвращаем предпоследнюю часть
    return parts[parts.size() - 2];
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

    
    std::vector<std::shared_ptr<Mesh>> groupMeshes;
    

    if (QFile::exists(filePath)) {
        qDebug() << "Load mesh from " << filePath;
        std::string name = getPenultimateWord(filePath);
        ObjLoader loader;
        std::vector<std::shared_ptr<Mesh>> meshes = loader.load(filePath.toStdString());
        // std::unordered_map<int, std::vector<std::shared_ptr<Mesh>>> objectGroups;
        for (const auto mesh: meshes) {
            openglWidget->addShape(mesh);
            mesh->setPosition(glm::vec3(0, 0, 0));
            mesh->setRotation(glm::vec3(0, 0, 0));
            mesh->setScale(glm::vec3(1, 1, 1));
            // addItemToList(i, name);
            groupMeshes.push_back(mesh);
        }
        objectGroups[groupId] = groupMeshes; // Сохраняем группу
        addItemToList(groupId, name);       // Привязываем группу к списку
        groupId++;
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

void MainWindow::addItemToList(int i, const std::string& type)
{   
    QListWidgetItem* newItem = new QListWidgetItem(QString::fromStdString(type), ui->listWidget);
    
    newItem->setData(Qt::UserRole, i);

    newItem->setData(Qt::UserRole + 1, QString::fromStdString(type));

    ui->listWidget->addItem(newItem);
}

void MainWindow::onObjectSelected(QListWidgetItem *item)
{
    int id = item->data(Qt::UserRole).toInt();

    QString type = item->data(Qt::UserRole + 1).toString();

    qDebug() << "Selected Item ID: " << id;
    qDebug() << "Item Type: " << type;

    std::shared_ptr<Shape> selectedShape;
    selectedShape = openglWidget->getAllShapes()[id];
    if (selectedShape) {
        QMessageBox dialog;
        dialog.setWindowTitle("Действия с объектом");
        dialog.setText("Выберите действие для объекта:");

        QPushButton *settingsButton = dialog.addButton("Настройки", QMessageBox::ActionRole);
        
        QPushButton *deleteButton = dialog.addButton("Удалить", QMessageBox::ActionRole);
        
        dialog.exec();

        if (dialog.clickedButton() == settingsButton) {
            showObjectSettings(selectedShape, id);
        } else if (dialog.clickedButton() == deleteButton) {
            if (!type.startsWith("cube")) {
                for (auto mesh: objectGroups[id]) {
                    openglWidget->eraseShape(mesh);
                    // std::cout << "Deleted mesh" << std::endl;
                }
                int row = ui->listWidget->row(item);
                ui->listWidget->takeItem(row);
                delete item;
                objectGroups.erase(id);
                groupId -= 1;
            } else {
                openglWidget->eraseShape(selectedShape);
                int row = ui->listWidget->row(item);
                ui->listWidget->takeItem(row);
                delete item; 
                i -= 1;
            }
            
        
        }
    }
}


void MainWindow::showObjectSettings(std::shared_ptr<Shape> shape, int groupId)
{
    QDialog settingsDialog;
    settingsDialog.setWindowTitle("Настройки объекта");
    settingsDialog.resize(400, 600);

    QVBoxLayout *layout = new QVBoxLayout();

    // Раздел для позиции
    QGroupBox *positionGroup = new QGroupBox("Позиция");
    QFormLayout *positionLayout = new QFormLayout;

    glm::vec3 position = shape->getPosition();
    QLineEdit *positionX = new QLineEdit(QString::number(position.x));
    QLineEdit *positionY = new QLineEdit(QString::number(position.y));
    QLineEdit *positionZ = new QLineEdit(QString::number(position.z));

    positionLayout->addRow("x:", positionX);
    positionLayout->addRow("y:", positionY);
    positionLayout->addRow("z:", positionZ);
    positionGroup->setLayout(positionLayout);
    layout->addWidget(positionGroup);

    // Раздел для Rotation
    QGroupBox *rotationGroup = new QGroupBox("Rotation");
    QFormLayout *rotationLayout = new QFormLayout;
    glm::vec3 rotation = shape->getRotation();

    QLabel *rotXlabel = new QLabel("x:", &settingsDialog);
    QSlider *rotXSlider = new QSlider(Qt::Horizontal, &settingsDialog);
    rotXSlider->setRange(0, 359);
    rotXSlider->setValue(static_cast<int>(rotation.x * 359)); // Значение от 0 до 255
    QLabel *rotXvalue = new QLabel(QString::number(rotation.x), &settingsDialog);
    connect(rotXSlider, &QSlider::valueChanged, [rotXvalue](int value) {
        rotXvalue->setText(QString::number(value / 359.0f, 'f', 2)); // Отображаем значение от 0 до 1
    });

    QLabel *rotYlabel = new QLabel("y:", &settingsDialog);
    QSlider *rotYSlider = new QSlider(Qt::Horizontal, &settingsDialog);
    rotYSlider->setRange(0, 359);
    rotYSlider->setValue(static_cast<int>(rotation.y * 359)); // Значение от 0 до 255
    QLabel *rotYvalue = new QLabel(QString::number(rotation.y), &settingsDialog);
    connect(rotYSlider, &QSlider::valueChanged, [rotYvalue](int value) {
        rotYvalue->setText(QString::number(value / 359.0f, 'f', 2)); // Отображаем значение от 0 до 1
    });

    QLabel *rotZlabel = new QLabel("z:", &settingsDialog);
    QSlider *rotZSlider = new QSlider(Qt::Horizontal, &settingsDialog);
    rotZSlider->setRange(0, 359);
    rotZSlider->setValue(static_cast<int>(rotation.z * 359)); // Значение от 0 до 255
    QLabel *rotZvalue = new QLabel(QString::number(rotation.z), &settingsDialog);
    connect(rotZSlider, &QSlider::valueChanged, [rotZvalue](int value) {
        rotZvalue->setText(QString::number(value / 359.0f, 'f', 2)); // Отображаем значение от 0 до 1
    });

    // Добавление компонентов в layout
    rotationLayout->addRow(rotXlabel, rotXSlider);
    rotationLayout->addRow("", rotXvalue); // Значение для r
    rotationLayout->addRow(rotYlabel, rotYSlider);
    rotationLayout->addRow("", rotYvalue); // Значение для g
    rotationLayout->addRow(rotZlabel, rotZSlider);
    rotationLayout->addRow("", rotZvalue); // Значение для b
    rotationGroup->setLayout(rotationLayout);
    layout->addWidget(rotationGroup);

    // Раздел для AmbientColor
    QGroupBox *ambientColorGroup = new QGroupBox("AmbientColor");
    QFormLayout *ambientColorLayout = new QFormLayout;
    glm::vec3 ambientColor = shape->getAmbientColor();

    // Красный компонент (R)
    QLabel *colorAXLabel = new QLabel("r:", &settingsDialog);
    QSlider *colorAXSlider = new QSlider(Qt::Horizontal, &settingsDialog);
    colorAXSlider->setRange(0, 255); // Диапазон для RGB (0-255)
    colorAXSlider->setValue(static_cast<int>(ambientColor.x * 255)); // Значение от 0 до 255
    QLabel *colorAXValue = new QLabel(QString::number(ambientColor.x), &settingsDialog);
    connect(colorAXSlider, &QSlider::valueChanged, [colorAXValue](int value) {
        colorAXValue->setText(QString::number(value / 255.0f, 'f', 2)); // Отображаем значение от 0 до 1
    });

    // Зеленый компонент (G)
    QLabel *colorAYLabel = new QLabel("g:", &settingsDialog);
    QSlider *colorAYSlider = new QSlider(Qt::Horizontal, &settingsDialog);
    colorAYSlider->setRange(0, 255);
    colorAYSlider->setValue(static_cast<int>(ambientColor.y * 255));
    QLabel *colorAYValue = new QLabel(QString::number(ambientColor.y), &settingsDialog);
    connect(colorAYSlider, &QSlider::valueChanged, [colorAYValue](int value) {
        colorAYValue->setText(QString::number(value / 255.0f, 'f', 2));
    });

    // Синий компонент (B)
    QLabel *colorAZLabel = new QLabel("b:", &settingsDialog);
    QSlider *colorAZSlider = new QSlider(Qt::Horizontal, &settingsDialog);
    colorAZSlider->setRange(0, 255);
    colorAZSlider->setValue(static_cast<int>(ambientColor.z * 255));
    QLabel *colorAZValue = new QLabel(QString::number(ambientColor.z), &settingsDialog);
    connect(colorAZSlider, &QSlider::valueChanged, [colorAZValue](int value) {
        colorAZValue->setText(QString::number(value / 255.0f, 'f', 2));
    });

    // Добавление компонентов в layout
    ambientColorLayout->addRow(colorAXLabel, colorAXSlider);
    ambientColorLayout->addRow("", colorAXValue); // Значение для r
    ambientColorLayout->addRow(colorAYLabel, colorAYSlider);
    ambientColorLayout->addRow("", colorAYValue); // Значение для g
    ambientColorLayout->addRow(colorAZLabel, colorAZSlider);
    ambientColorLayout->addRow("", colorAZValue); // Значение для b
    ambientColorGroup->setLayout(ambientColorLayout);
    layout->addWidget(ambientColorGroup);

    // Раздел для DiffuseColor
    QGroupBox *diffuseColorGroup = new QGroupBox("DiffuseColor");
    QFormLayout *diffuseColorLayout = new QFormLayout;
    glm::vec3 diffuseColor = shape->getDiffuseColor();

    // Красный компонент (R)
    QLabel *colorDXLabel = new QLabel("r:", &settingsDialog);
    QSlider *colorDXSlider = new QSlider(Qt::Horizontal, &settingsDialog);
    colorDXSlider->setRange(0, 255); // Диапазон для RGB (0-255)
    colorDXSlider->setValue(static_cast<int>(diffuseColor.x * 255)); // Значение от 0 до 255
    QLabel *colorDXValue = new QLabel(QString::number(diffuseColor.x), &settingsDialog);
    connect(colorDXSlider, &QSlider::valueChanged, [colorDXValue](int value) {
        colorDXValue->setText(QString::number(value / 255.0f, 'f', 2)); // Отображаем значение от 0 до 1
    });

    // Зеленый компонент (G)
    QLabel *colorDYLabel = new QLabel("g:", &settingsDialog);
    QSlider *colorDYSlider = new QSlider(Qt::Horizontal, &settingsDialog);
    colorDYSlider->setRange(0, 255);
    colorDYSlider->setValue(static_cast<int>(diffuseColor.y * 255));
    QLabel *colorDYValue = new QLabel(QString::number(diffuseColor.y), &settingsDialog);
    connect(colorDYSlider, &QSlider::valueChanged, [colorDYValue](int value) {
        colorDYValue->setText(QString::number(value / 255.0f, 'f', 2));
    });

    // Синий компонент (B)
    QLabel *colorDZLabel = new QLabel("b:", &settingsDialog);
    QSlider *colorDZSlider = new QSlider(Qt::Horizontal, &settingsDialog);
    colorDZSlider->setRange(0, 255);
    colorDZSlider->setValue(static_cast<int>(diffuseColor.z * 255));
    QLabel *colorDZValue = new QLabel(QString::number(diffuseColor.z), &settingsDialog);
    connect(colorDZSlider, &QSlider::valueChanged, [colorDZValue](int value) {
        colorDZValue->setText(QString::number(value / 255.0f, 'f', 2));
    });

    // Добавление компонентов в layout
    diffuseColorLayout->addRow(colorDXLabel, colorDXSlider);
    diffuseColorLayout->addRow("", colorDXValue); // Значение для r
    diffuseColorLayout->addRow(colorDYLabel, colorDYSlider);
    diffuseColorLayout->addRow("", colorDYValue); // Значение для g
    diffuseColorLayout->addRow(colorDZLabel, colorDZSlider);
    diffuseColorLayout->addRow("", colorDZValue); // Значение для b
    diffuseColorGroup->setLayout(diffuseColorLayout);
    layout->addWidget(diffuseColorGroup);

    // Раздел для SpecularColor
    QGroupBox *specularColorGroup = new QGroupBox("SpecularColor");
    QFormLayout *specularColorLayout = new QFormLayout;
    glm::vec3 specularColor = shape->getSpecularColor();

    // Красный компонент (R)
    QLabel *colorSXLabel = new QLabel("r:", &settingsDialog);
    QSlider *colorSXSlider = new QSlider(Qt::Horizontal, &settingsDialog);
    colorSXSlider->setRange(0, 255); // Диапазон для RGB (0-255)
    colorSXSlider->setValue(static_cast<int>(specularColor.x * 255)); // Значение от 0 до 255
    QLabel *colorSXValue = new QLabel(QString::number(specularColor.x), &settingsDialog);
    connect(colorSXSlider, &QSlider::valueChanged, [colorSXValue](int value) {
        colorSXValue->setText(QString::number(value / 255.0f, 'f', 2)); // Отображаем значение от 0 до 1
    });

    // Зеленый компонент (G)
    QLabel *colorSYLabel = new QLabel("g:", &settingsDialog);
    QSlider *colorSYSlider = new QSlider(Qt::Horizontal, &settingsDialog);
    colorSYSlider->setRange(0, 255);
    colorSYSlider->setValue(static_cast<int>(specularColor.y * 255));
    QLabel *colorSYValue = new QLabel(QString::number(specularColor.y), &settingsDialog);
    connect(colorSYSlider, &QSlider::valueChanged, [colorSYValue](int value) {
        colorSYValue->setText(QString::number(value / 255.0f, 'f', 2));
    });

    // Синий компонент (B)
    QLabel *colorSZLabel = new QLabel("b:", &settingsDialog);
    QSlider *colorSZSlider = new QSlider(Qt::Horizontal, &settingsDialog);
    colorSZSlider->setRange(0, 255);
    colorSZSlider->setValue(static_cast<int>(specularColor.z * 255));
    QLabel *colorSZValue = new QLabel(QString::number(specularColor.z), &settingsDialog);
    connect(colorSZSlider, &QSlider::valueChanged, [colorSZValue](int value) {
        colorSZValue->setText(QString::number(value / 255.0f, 'f', 2));
    });

    // Добавление компонентов в layout
    specularColorLayout->addRow(colorSXLabel, colorSXSlider);
    specularColorLayout->addRow("", colorSXValue); // Значение для r
    specularColorLayout->addRow(colorSYLabel, colorSYSlider);
    specularColorLayout->addRow("", colorSYValue); // Значение для g
    specularColorLayout->addRow(colorSZLabel, colorSZSlider);
    specularColorLayout->addRow("", colorSZValue); // Значение для b
    specularColorGroup->setLayout(specularColorLayout);
    layout->addWidget(specularColorGroup);

    
    QGroupBox *shininessGroup = new QGroupBox("ShininessColor");
    QFormLayout *shininessLayout = new QFormLayout;
    float shininess = shape->getShininess();
    QLineEdit *shininessX = new QLineEdit(QString::number(shininess));    
    shininessLayout->addRow("shininess:", shininessX);
    shininessGroup->setLayout(shininessLayout);
    layout->addWidget(shininessGroup);

    QGroupBox *scaleGroup =  new QGroupBox("Scale");
    QFormLayout *scaleLayout = new QFormLayout;
    glm::vec3 scale = shape->getScale();
    QLineEdit *vecX = new QLineEdit(QString::number(scale.x));
    QLineEdit *vecY = new QLineEdit(QString::number(scale.y));
    QLineEdit *vecZ = new QLineEdit(QString::number(scale.z));
    scaleLayout->addRow("x:", vecX);
    scaleLayout->addRow("y:", vecY);
    scaleLayout->addRow("z:", vecZ);
    scaleGroup->setLayout(scaleLayout);
    layout->addWidget(scaleGroup);

    // Кнопка сохранения
    QPushButton *saveButton = new QPushButton("Сохранить");
    connect(saveButton, &QPushButton::clicked, [&]() {
         if (shape->getType() == "mesh") {
            for (auto& mesh: objectGroups[groupId]) {
                glm::vec3 mesh_pos = mesh->getPosition();
                mesh->setPosition(glm::vec3(mesh_pos.x + positionX->text().toFloat(), mesh_pos.y + positionY->text().toFloat(), mesh_pos.z + positionZ->text().toFloat()));
            }
        } else {
            shape->setPosition(glm::vec3(positionX->text().toFloat(), positionY->text().toFloat(), positionZ->text().toFloat()));
        }
        
        shape->setAmbientColor(glm::vec3(colorAXSlider->value() / 255.0f, colorAYSlider->value() / 255.0f, colorAZSlider->value() / 255.0f));
        shape->setDiffuseColor(glm::vec3(colorDXSlider->value() / 255.0f, colorDYSlider->value() / 255.0f, colorDZSlider->value() / 255.0f));
        shape->setSpecularColor(glm::vec3(colorSXSlider->value() / 255.0f, colorSYSlider->value() / 255.0f, colorSZSlider->value() / 255.0f));
        shape->setScale(glm::vec3(vecX->text().toFloat(), vecY->text().toFloat(), vecZ->text().toFloat()));
        shape->setShininess(shininessX->text().toFloat());
        shape->setRotation(glm::vec3(rotXSlider->value(), rotYSlider->value(), rotZSlider->value()));

        settingsDialog.accept();
    });
    layout->addWidget(saveButton);

    settingsDialog.setLayout(layout);
    settingsDialog.exec();
}

MainWindow::~MainWindow()
{
    delete ui;
}
