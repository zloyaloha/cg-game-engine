#include "app.h"
#include "./ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->addCube, &QPushButton::clicked, this, &MainWindow::addCubeButtonClicked);
    connect(ui->addMesh, &QPushButton::clicked, this, &MainWindow::addMeshButtonClicked);
    connect(ui->changeProjection, &QPushButton::clicked, this, &MainWindow::changeProjectionButtonClicked);
    connect(ui->startScene, &QPushButton::clicked, this, &MainWindow::startScene);
    connect(ui->restorePosition, &QPushButton::clicked, this, &MainWindow::restorePosition);
    connect(ui->listWidget, &QListWidget::itemClicked, this, &MainWindow::onObjectSelected);
    setWindowTitle("Игры Разума");

    openglWidget = new OpenGLWidget(ui->openGLWidget);
    openglWidget->resize(1080, 720);


    connect(ui->listWidget, &QListWidget::itemClicked, this, &MainWindow::onObjectSelected);

    // Work With Light
    connect(ui->addPointLight, &QPushButton::clicked, this, &MainWindow::addPointLightButtonClicked);
    connect(ui->addSpotLight, &QPushButton::clicked, this, &MainWindow::addSpotLightButtonClicked);
    connect(ui->addDirectionalLight, &QPushButton::clicked, this, &MainWindow::addDirectionalLightButtonClicked);

    connect(ui->lightSources, &QListWidget::itemClicked, this, &MainWindow::onLightSourceSelected);
    fpsUpdateTimer = std::make_shared<QTimer>(this);
    connect(fpsUpdateTimer.get(), &QTimer::timeout, this, &MainWindow::displayFPS);
    fpsUpdateTimer->start(1000);
}

void MainWindow::addItemToLightList(int i, const std::string& light_type)
{
    // Создаем новый элемент списка
    QListWidgetItem* new_light_item = new QListWidgetItem(QString::fromStdString(light_type), ui->lightSources);

    // Привязываем числовое значение к элементу
    new_light_item->setData(Qt::UserRole, i);  // Qt::UserRole можно использовать для хранения целых значений

    // Можно также привязать тип (строку) как дополнительные данные
    new_light_item->setData(Qt::UserRole + 1, QString::fromStdString(light_type));

    // Добавляем элемент в список
    ui->lightSources->addItem(new_light_item);
}

void MainWindow::addPointLightButtonClicked()
{
    addItemToLightList(counterLights, "Point Light");
    std::shared_ptr<PointLight> point_light = std::make_shared<PointLight>(
        glm::vec3(-10.0f, -10.0f, -10.0f) // Позиция источника света
    );

    openglWidget->addLight(point_light);

    counterLights += 1;
}

void MainWindow::addSpotLightButtonClicked()
{
    addItemToLightList(counterLights, "Spot Light");
    std::shared_ptr<SpotLight> spot_light = std::make_shared<SpotLight>(
        glm::vec3(5.0f, 5.0f, 5.0f), // Позиция прожектора
        glm::vec3(1.0f, 1.0f, 1.0f)  // Направление прожектора
    );

    openglWidget->addLight(spot_light);

    counterLights += 1;
}

void MainWindow::addDirectionalLightButtonClicked()
{
    addItemToLightList(counterLights, "Directional Light");
    std::shared_ptr<DirectionalLight> directional_light = std::make_shared<DirectionalLight>(
        glm::vec3(0.0f, -1.0f, 0.0f) // Направление света (например, вниз)
    );

    openglWidget->addLight(directional_light);

    counterLights += 1;
}

void MainWindow::onLightSourceSelected(QListWidgetItem *light_source)
{
    // Извлекаем числовое значение (например, ID)
    int id = light_source->data(Qt::UserRole).toInt();

    // Извлекаем строку с типом
    QString type = light_source->data(Qt::UserRole + 1).toString();

    // Выводим данные для проверки
    qDebug() << "Selected Light Source ID: " << id;
    qDebug() << "Light Source Type: " << type;

    // Например, если у вас есть указатель на объект Shape, привязанный к этим данным
    // Получаем объект Shape, например, из списка или другой структуры данных
    std::shared_ptr<Light> selected_light_source;
    selected_light_source = openglWidget->getAllLights()[id];
    if (selected_light_source) {
        Light::LightType light_type = selected_light_source->getType();

        if (light_type == Light::POINT) {
            qDebug() << "Point Light Settings";
            showPointLightSourceSettings(selected_light_source);
        } else if (light_type == Light::SPOT) {
            qDebug() << "Spot Light Settings";
            showSpotLightSourceSettings(selected_light_source);
        } else if (light_type == Light::DIRECTIONAL) {
            qDebug() << "It works 3";
            showDirectionalLightSourceSettings(selected_light_source);
        }
    }
}

void MainWindow::showPointLightSourceSettings(std::shared_ptr<Light> pointLight)
{
    auto point_light_source = std::dynamic_pointer_cast<PointLight>(pointLight);

    QDialog settingsDialog;
    settingsDialog.setWindowTitle("Point Light Source Settings");
    settingsDialog.resize(300, 400);

    QVBoxLayout *layout = new QVBoxLayout;

    // Параметры позиции
    QGroupBox *positionGroup = new QGroupBox("Position");
    QFormLayout *positionLayout = new QFormLayout;
    QLineEdit *posX = new QLineEdit(QString::number(point_light_source->position.x));
    QLineEdit *posY = new QLineEdit(QString::number(point_light_source->position.y));
    QLineEdit *posZ = new QLineEdit(QString::number(point_light_source->position.z));
    positionLayout->addRow("X:", posX);
    positionLayout->addRow("Y:", posY);
    positionLayout->addRow("Z:", posZ);
    positionGroup->setLayout(positionLayout);
    layout->addWidget(positionGroup);

    // Параметры цвета
    QGroupBox *colorGroup = new QGroupBox("Color");
    QFormLayout *colorLayout = new QFormLayout;
    QLineEdit *colorR = new QLineEdit(QString::number(point_light_source->color.r));
    QLineEdit *colorG = new QLineEdit(QString::number(point_light_source->color.g));
    QLineEdit *colorB = new QLineEdit(QString::number(point_light_source->color.b));
    colorLayout->addRow("R:", colorR);
    colorLayout->addRow("G:", colorG);
    colorLayout->addRow("B:", colorB);
    colorGroup->setLayout(colorLayout);
    layout->addWidget(colorGroup);

    // Параметры затухания
    QGroupBox *attenuationGroup = new QGroupBox("Attenuation");
    QFormLayout *attenuationLayout = new QFormLayout;
    QLineEdit *constant = new QLineEdit(QString::number(point_light_source->constant));
    QLineEdit *linear = new QLineEdit(QString::number(point_light_source->linear));
    QLineEdit *quadratic = new QLineEdit(QString::number(point_light_source->quadratic));
    attenuationLayout->addRow("Constant:", constant);
    attenuationLayout->addRow("Linear:", linear);
    attenuationLayout->addRow("Quadratic:", quadratic);
    attenuationGroup->setLayout(attenuationLayout);
    layout->addWidget(attenuationGroup);

    // Кнопка сохранения
    QPushButton *saveButton = new QPushButton("Save");
    connect(saveButton, &QPushButton::clicked, [&]() {
        point_light_source->position = glm::vec3(posX->text().toFloat(), posY->text().toFloat(), posZ->text().toFloat());
        point_light_source->color = glm::vec3(colorR->text().toFloat(), colorG->text().toFloat(), colorB->text().toFloat());
        point_light_source->constant = constant->text().toFloat();
        point_light_source->linear = linear->text().toFloat();
        point_light_source->quadratic = quadratic->text().toFloat();
        settingsDialog.accept();
    });
    layout->addWidget(saveButton);

    settingsDialog.setLayout(layout);
    settingsDialog.exec();
}

void MainWindow::showSpotLightSourceSettings(std::shared_ptr<Light> spotLight)
{
    auto spot_light_source = std::dynamic_pointer_cast<SpotLight>(spotLight);

    QDialog settingsDialog;
    settingsDialog.setWindowTitle("Spot Light Source Settings");
    settingsDialog.resize(300, 500);

    QVBoxLayout *layout = new QVBoxLayout;

    // Параметры позиции
    QGroupBox *positionGroup = new QGroupBox("Position");
    QFormLayout *positionLayout = new QFormLayout;
    QLineEdit *posX = new QLineEdit(QString::number(spot_light_source->position.x));
    QLineEdit *posY = new QLineEdit(QString::number(spot_light_source->position.y));
    QLineEdit *posZ = new QLineEdit(QString::number(spot_light_source->position.z));
    positionLayout->addRow("X:", posX);
    positionLayout->addRow("Y:", posY);
    positionLayout->addRow("Z:", posZ);
    positionGroup->setLayout(positionLayout);
    layout->addWidget(positionGroup);

    // Параметры направления
    QGroupBox *directionGroup = new QGroupBox("Direction");
    QFormLayout *directionLayout = new QFormLayout;
    QLineEdit *dirX = new QLineEdit(QString::number(spot_light_source->direction.x));
    QLineEdit *dirY = new QLineEdit(QString::number(spot_light_source->direction.y));
    QLineEdit *dirZ = new QLineEdit(QString::number(spot_light_source->direction.z));
    directionLayout->addRow("X:", dirX);
    directionLayout->addRow("Y:", dirY);
    directionLayout->addRow("Z:", dirZ);
    directionGroup->setLayout(directionLayout);
    layout->addWidget(directionGroup);

    // Параметры углов
    QGroupBox *angleGroup = new QGroupBox("Angles");
    QFormLayout *angleLayout = new QFormLayout;
    QLineEdit *cutOff = new QLineEdit(QString::number(glm::degrees(glm::acos(spot_light_source->cutOff))));
    QLineEdit *outerCutOff = new QLineEdit(QString::number(glm::degrees(glm::acos(spot_light_source->outerCutOff))));
    angleLayout->addRow("Cut-off (degrees):", cutOff);
    angleLayout->addRow("Outer Cut-off (degrees):", outerCutOff);
    angleGroup->setLayout(angleLayout);
    layout->addWidget(angleGroup);

    // Параметры цвета
    QGroupBox *colorGroup = new QGroupBox("Color");
    QFormLayout *colorLayout = new QFormLayout;
    QLineEdit *colorR = new QLineEdit(QString::number(spot_light_source->color.r));
    QLineEdit *colorG = new QLineEdit(QString::number(spot_light_source->color.g));
    QLineEdit *colorB = new QLineEdit(QString::number(spot_light_source->color.b));
    colorLayout->addRow("R:", colorR);
    colorLayout->addRow("G:", colorG);
    colorLayout->addRow("B:", colorB);
    colorGroup->setLayout(colorLayout);
    layout->addWidget(colorGroup);

    // Параметры интенсивности
    QGroupBox *intensityGroup = new QGroupBox("Intensity");
    QFormLayout *intensityLayout = new QFormLayout;
    QLineEdit *intensity = new QLineEdit(QString::number(spot_light_source->intensity));
    intensityLayout->addRow("Intensity:", intensity);
    intensityGroup->setLayout(intensityLayout);
    layout->addWidget(intensityGroup);

    // Кнопка сохранения
    QPushButton *saveButton = new QPushButton("Save");
    connect(saveButton, &QPushButton::clicked, [&]() {
        spot_light_source->position = glm::vec3(posX->text().toFloat(), posY->text().toFloat(), posZ->text().toFloat());
        spot_light_source->direction = glm::vec3(dirX->text().toFloat(), dirY->text().toFloat(), dirZ->text().toFloat());
        spot_light_source->cutOff = glm::cos(glm::radians(cutOff->text().toFloat()));
        spot_light_source->outerCutOff = glm::cos(glm::radians(outerCutOff->text().toFloat()));
        spot_light_source->color = glm::vec3(colorR->text().toFloat(), colorG->text().toFloat(), colorB->text().toFloat());
        spot_light_source->intensity = intensity->text().toFloat();
        settingsDialog.accept();
    });
    layout->addWidget(saveButton);

    settingsDialog.setLayout(layout);
    settingsDialog.exec();
}

void MainWindow::showDirectionalLightSourceSettings(std::shared_ptr<Light> directionalLight)
{
    auto directional_light_source = std::dynamic_pointer_cast<DirectionalLight>(directionalLight);

    QDialog settingsDialog;
    settingsDialog.setWindowTitle("Directional Light Source Settings");
    settingsDialog.resize(300, 400);

    QVBoxLayout *layout = new QVBoxLayout;

    // Параметры направления
    QGroupBox *directionGroup = new QGroupBox("Direction");
    QFormLayout *directionLayout = new QFormLayout;
    QLineEdit *dirX = new QLineEdit(QString::number(directional_light_source->direction.x));
    QLineEdit *dirY = new QLineEdit(QString::number(directional_light_source->direction.y));
    QLineEdit *dirZ = new QLineEdit(QString::number(directional_light_source->direction.z));
    directionLayout->addRow("X:", dirX);
    directionLayout->addRow("Y:", dirY);
    directionLayout->addRow("Z:", dirZ);
    directionGroup->setLayout(directionLayout);
    layout->addWidget(directionGroup);

    // Параметры цвета
    QGroupBox *colorGroup = new QGroupBox("Color");
    QFormLayout *colorLayout = new QFormLayout;
    QLineEdit *colorR = new QLineEdit(QString::number(directional_light_source->color.r));
    QLineEdit *colorG = new QLineEdit(QString::number(directional_light_source->color.g));
    QLineEdit *colorB = new QLineEdit(QString::number(directional_light_source->color.b));
    colorLayout->addRow("R:", colorR);
    colorLayout->addRow("G:", colorG);
    colorLayout->addRow("B:", colorB);
    colorGroup->setLayout(colorLayout);
    layout->addWidget(colorGroup);

    // Параметры интенсивности
    QGroupBox *intensityGroup = new QGroupBox("Intensity");
    QFormLayout *intensityLayout = new QFormLayout;
    QLineEdit *intensity = new QLineEdit(QString::number(directional_light_source->intensity));
    intensityLayout->addRow("Intensity:", intensity);
    intensityGroup->setLayout(intensityLayout);
    layout->addWidget(intensityGroup);

    // Кнопка сохранения
    QPushButton *saveButton = new QPushButton("Save");
    connect(saveButton, &QPushButton::clicked, [&]() {
        directional_light_source->direction = glm::vec3(dirX->text().toFloat(), dirY->text().toFloat(), dirZ->text().toFloat());
        directional_light_source->color = glm::vec3(colorR->text().toFloat(), colorG->text().toFloat(), colorB->text().toFloat());
        directional_light_source->intensity = intensity->text().toFloat();
        settingsDialog.accept();
    });
    layout->addWidget(saveButton);

    settingsDialog.setLayout(layout);
    settingsDialog.exec();
}

void MainWindow::addCubeButtonClicked()
{
    std::shared_ptr<Cube> cube = std::make_shared<Cube>(1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
    std::shared_ptr<Material> material = std::make_shared<Material>();
    material->ambientColor = glm::vec3(1, 1, 1);
    material->diffuseColor = glm::vec3(0.4f, 0.4f, 0.4f);
    material->specularColor = glm::vec3(0.7f, 0.7f, 0.7f);
    material->shininess = 10.0f;
    cube->setRotation(glm::vec3(0, 0, 0));
    cube->setScale(glm::vec3(1, 1, 1));
    cube->setMaterial(material);
    std::string name = "cube" + std::to_string(counterObjs);
    addItemToList(name, {counterObjs});
    openglWidget->addShape(cube);
    counterObjs += 1;
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
        QList<int> list;
        for (const auto mesh: meshes) {
            openglWidget->addShape(mesh);
            mesh->setPosition(glm::vec3(0, 0, 0));
            mesh->setRotation(glm::vec3(0, 0, 0));
            mesh->setScale(glm::vec3(1, 1, 1));
            list.append(counterObjs);
            ++counterObjs;
        }
        addItemToList(name, list);
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

void MainWindow::addItemToList(const std::string& type, const QVector<int>& objectIds)
{
    QListWidgetItem* newItem = new QListWidgetItem(QString::fromStdString(type), ui->listWidget);

    newItem->setData(Qt::UserRole, QString::fromStdString(type));

    QVariantList objectList;
    for (int objectId : objectIds) {
        objectList.append(objectId);
    }
    newItem->setData(Qt::UserRole + 1, objectList);

    ui->listWidget->addItem(newItem);
}

void MainWindow::onObjectSelected(QListWidgetItem *item)
{
    QString type = item->data(Qt::UserRole).toString();
    QVariantList objectList = item->data(Qt::UserRole + 1).toList();

    if (objectList.isEmpty()) {
        qDebug() << "Ошибка: objectList пуст";
        return;
    }
    int id = objectList[0].toInt();

    auto shapes = openglWidget->getAllShapes();
    if (id < 0 || id >= shapes.size()) {
        return;
    }

    std::shared_ptr<Shape> selectedShape = shapes[id];

    QMenu contextMenu(this);

    QAction *settingsAction = contextMenu.addAction("Настройки");
    QAction *deleteAction = contextMenu.addAction("Удалить");

    // Показ меню под курсором
    QAction *selectedAction = contextMenu.exec(QCursor::pos());

    if (!selectedAction) {
        return;
    }

    if (selectedAction == settingsAction) {
        showObjectSettings(selectedShape, id);
    } else if (selectedAction == deleteAction) {
        if (!type.startsWith("cube")) {

            for (const QVariant& var : objectList) {
                openglWidget->eraseShape(var.toInt());
            }

            int row = ui->listWidget->row(item);
            if (row < 0) {
                qDebug() << "Ошибка: item не найден в listWidget";
                return;
            }

            ui->listWidget->takeItem(row);
            counterObjs -= 1;
        } else {
            openglWidget->eraseShape(id);

            int row = ui->listWidget->row(item);
            if (row < 0) {
                qDebug() << "Ошибка: item не найден в listWidget";
                return;
            }

            ui->listWidget->takeItem(row);
            counterObjs -= 1;
        }
    }
}

void MainWindow::showObjectSettings(std::shared_ptr<Shape> shape, int groupId)
{
    QDialog settingsDialog;
    settingsDialog.setWindowTitle("Настройки объекта");
    settingsDialog.resize(400, 600);

    QGridLayout  *layout = new QGridLayout ();

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
    layout->addWidget(positionGroup, 0, 0);

    QGroupBox *velocityGroup = new QGroupBox("Скорость");
    QFormLayout *velocityLayout = new QFormLayout;
    glm::vec3 velocity = shape->getVelocity();
    QLineEdit *velocityX = new QLineEdit(QString::number(velocity.x));
    QLineEdit *velocityY = new QLineEdit(QString::number(velocity.y));
    QLineEdit *velocityZ = new QLineEdit(QString::number(velocity.z));

    velocityLayout->addRow("x:", velocityX);
    velocityLayout->addRow("y:", velocityY);
    velocityLayout->addRow("z:", velocityZ);
    velocityGroup->setLayout(velocityLayout);
    layout->addWidget(velocityGroup, 2, 0);

    QGroupBox *accelerationGroup = new QGroupBox("Ускорение");
    QFormLayout *accelerationLayout = new QFormLayout;
    glm::vec3 acceleration = shape->getAcceleration();
    QLineEdit *accelerationX = new QLineEdit(QString::number(acceleration.x));
    QLineEdit *accelerationY = new QLineEdit(QString::number(acceleration.y));
    QLineEdit *accelerationZ = new QLineEdit(QString::number(acceleration.z));

    accelerationLayout->addRow("x:", accelerationX);
    accelerationLayout->addRow("y:", accelerationY);
    accelerationLayout->addRow("z:", accelerationZ);
    accelerationGroup->setLayout(accelerationLayout);
    layout->addWidget(accelerationGroup, 2, 1);

    QGroupBox *parametersGroup = new QGroupBox("Параметры");
    QFormLayout *parametersLayout = new QFormLayout;
    float mass = shape->getMass();
    bool ghost = shape->isGhost();
    QLineEdit *massLine = new QLineEdit(QString::number(mass));
    QCheckBox *isGhost = new QCheckBox();
    isGhost->setChecked(ghost);

    parametersLayout->addRow("Масса:", massLine);
    parametersLayout->addRow("Призрак:", isGhost);
    parametersGroup->setLayout(parametersLayout);
    layout->addWidget(parametersGroup, 2, 2);

    // Раздел для Rotation
    QGroupBox *rotationGroup = new QGroupBox("Поворот");
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
    layout->addWidget(rotationGroup, 0, 1);

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
    layout->addWidget(ambientColorGroup, 1, 0);

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
    layout->addWidget(diffuseColorGroup, 1, 1);

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
    layout->addWidget(specularColorGroup, 1, 2);

    QGroupBox *shininessGroup = new QGroupBox("ShininessColor");
    QFormLayout *shininessLayout = new QFormLayout;
    float shininess = shape->getShininess();
    QLineEdit *shininessX = new QLineEdit(QString::number(shininess));
    shininessLayout->addRow("shininess:", shininessX);
    shininessGroup->setLayout(shininessLayout);
    layout->addWidget(shininessGroup, 1, 3);

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
    layout->addWidget(scaleGroup, 0, 2);

    // Кнопка сохранения
    QPushButton *saveButton = new QPushButton("Сохранить");
    connect(saveButton, &QPushButton::clicked, [&]() {
         if (shape->getType() == "mesh") {
            for (auto& mesh: objectGroups[groupId]) {
                glm::vec3 mesh_pos = mesh->getPosition();
                mesh->setPosition(glm::vec3(mesh_pos.x + positionX->text().toFloat(), mesh_pos.y + positionY->text().toFloat(), mesh_pos.z + positionZ->text().toFloat()));
                mesh->setAmbientColor(glm::vec3(colorAXSlider->value() / 255.0f, colorAYSlider->value() / 255.0f, colorAZSlider->value() / 255.0f));
                mesh->setDiffuseColor(glm::vec3(colorDXSlider->value() / 255.0f, colorDYSlider->value() / 255.0f, colorDZSlider->value() / 255.0f));
                mesh->setSpecularColor(glm::vec3(colorSXSlider->value() / 255.0f, colorSYSlider->value() / 255.0f, colorSZSlider->value() / 255.0f));
                mesh->setScale(glm::vec3(vecX->text().toFloat(), vecY->text().toFloat(), vecZ->text().toFloat()));
                mesh->setShininess(shininessX->text().toFloat());
                mesh->setVelocity(glm::vec3(velocityX->text().toFloat(), velocityY->text().toFloat(), velocityZ->text().toFloat()));
                mesh->setAcceleration(glm::vec3(accelerationX->text().toFloat(), accelerationY->text().toFloat(), accelerationZ->text().toFloat()));
                mesh->setMass(mass);
                mesh->setGhostRegime(ghost);
            }
        }
        shape->setPosition(glm::vec3(positionX->text().toFloat(), positionY->text().toFloat(), positionZ->text().toFloat()));
        shape->setAmbientColor(glm::vec3(colorAXSlider->value() / 255.0f, colorAYSlider->value() / 255.0f, colorAZSlider->value() / 255.0f));
        shape->setDiffuseColor(glm::vec3(colorDXSlider->value() / 255.0f, colorDYSlider->value() / 255.0f, colorDZSlider->value() / 255.0f));
        shape->setSpecularColor(glm::vec3(colorSXSlider->value() / 255.0f, colorSYSlider->value() / 255.0f, colorSZSlider->value() / 255.0f));
        shape->setScale(glm::vec3(vecX->text().toFloat(), vecY->text().toFloat(), vecZ->text().toFloat()));
        shape->setShininess(shininessX->text().toFloat());
        shape->setVelocity(glm::vec3(velocityX->text().toFloat(), velocityY->text().toFloat(), velocityZ->text().toFloat()));
        shape->setAcceleration(glm::vec3(accelerationX->text().toFloat(), accelerationY->text().toFloat(), accelerationZ->text().toFloat()));
        shape->setMass(massLine->text().toFloat());
        shape->setGhostRegime(isGhost->isChecked());
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
