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

    // Work With Light
    connect(ui->addPointLight, &QPushButton::clicked, this, &MainWindow::addPointLightButtonClicked);
    connect(ui->addSpotLight, &QPushButton::clicked, this, &MainWindow::addSpotLightButtonClicked);
    connect(ui->addDirectionalLight, &QPushButton::clicked, this, &MainWindow::addDirectionalLightButtonClicked);

    connect(ui->lightSources, &QListWidget::itemClicked, this, &MainWindow::onLightSourceSelected);
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
    addItemToLightList(i, "Point Light");
    std::shared_ptr<PointLight> point_light = std::make_shared<PointLight>(
        glm::vec3(-10.0f, -10.0f, -10.0f) // Позиция источника света
    );

    openglWidget->addLight(point_light);

    i += 1;
}

void MainWindow::addSpotLightButtonClicked()
{
    addItemToLightList(i, "Spot Light");
    std::shared_ptr<SpotLight> spot_light = std::make_shared<SpotLight>(
        glm::vec3(5.0f, 5.0f, 5.0f), // Позиция прожектора
        glm::vec3(1.0f, 1.0f, 1.0f)  // Направление прожектора
    );

    openglWidget->addLight(spot_light);

    i += 1;
}

void MainWindow::addDirectionalLightButtonClicked()
{
    addItemToLightList(i, "Directional Light");
    std::shared_ptr<DirectionalLight> directional_light = std::make_shared<DirectionalLight>(
        glm::vec3(0.0f, -1.0f, 0.0f) // Направление света (например, вниз)
    );

    openglWidget->addLight(directional_light);

    i += 1;
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
    addItemToList(i, "cube");
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
            mesh->setPosition(glm::vec3(0, 0, 0));
            mesh->setRotation(glm::vec3(0, 0, 0));
            mesh->setScale(glm::vec3(1, 1, 1));
            addItemToList(i, "mesh");

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
    settingsDialog.resize(400, 600);



    // // Компоновка
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


    

    // Кнопка сохранения
    QPushButton *saveButton = new QPushButton("Сохранить");
    connect(saveButton, &QPushButton::clicked, [&]() {
        shape->setPosition(glm::vec3(positionX->text().toFloat(), positionY->text().toFloat(), positionZ->text().toFloat()));
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
