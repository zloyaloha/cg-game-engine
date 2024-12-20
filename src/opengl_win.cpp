#include "opengl_win.h"
#include <iostream>
#include <QFont>

OpenGLWidget::OpenGLWidget(QWidget *parent) : QOpenGLWidget(parent),
    camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f))
{
    aspectRatio = float(height()) / float(width());
    setFocusPolicy(Qt::StrongFocus);
    this->setFocus();
    std::fill(std::begin(keys), std::end(keys), false);
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(4, 2);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

    fpsUpdateTimer = new QTimer(this);
    connect(fpsUpdateTimer, &QTimer::timeout, this, &OpenGLWidget::updateFPS);
    fpsUpdateTimer->start(1000);  // Обновление FPS каждую секунду
}

void OpenGLWidget::addShape(std::shared_ptr<Shape> shape)
{
    shape->setShader(shaders[shape->getType()]);
    shape->setVao(vaos[shape->getType()]);
    shape->setProjectionMatrix(camera.getProjectionMatrix(aspectRatio));
    shape->setViewMatrix(camera.getViewMatrix());
    shape->setAcceleration(glm::vec3(0,0,0));
    shape->initialize();
    shapes.push_back(shape);
    setLigths();
    update();
    if (shape->getType() == "mesh") {
        shape->setVelocity(glm::vec3(0, 0, 0.01));
        shape->setMass(2);
        shape->changeGhostRegime();
    } else {
        shape->setPosition(glm::vec3(-2.5, 0, 6));
        shape->setVelocity(glm::vec3(0, 0, 0));
        shape->setMass(3);
        shape->changeGhostRegime();
    }
    shaders[shape->getType()]->release();
}

void OpenGLWidget::addLight(std::shared_ptr<Light> light)
{
    lights.push_back(light);
    setLigths();
}

std::vector<std::shared_ptr<Shape>> OpenGLWidget::getAllShapes()
{
    return shapes;
}

void OpenGLWidget::eraseShape(std::shared_ptr<Shape> shape)
{
    auto it = std::find(this->shapes.begin(), this->shapes.end(), shape);
    if (it != this->shapes.end()) {
        this->shapes.erase(it);
    }
}

void OpenGLWidget::updateFPS()
{
    float currentTime = static_cast<float>(QTime::currentTime().msecsSinceStartOfDay());
    deltaTime = currentTime - lastTime;
    lastTime = currentTime;
    if (deltaTime > 0.0f) {
        FPS = frameCount;
        frameCount = 0;
    }
}

int OpenGLWidget::getFPS()
{
    return FPS;
}

void OpenGLWidget::startScene()
{
    if (!started) {
        for (auto shape: shapes) {
            shape->saveState();
        }
    }
    started = !started;
}

void OpenGLWidget::changeCameraProjection()
{
    camera.changeProjection();
}

void OpenGLWidget::restorePosition()
{
    for (auto shape: shapes) {
        shape->restoreState();
    }
}

void OpenGLWidget::createShaders()
{
    shaders["cube"] = std::make_shared<QOpenGLShaderProgram>();
    shaders["cube"]->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/cubeVertexShader.glsl");
    shaders["cube"]->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/cubeFragmentShader.glsl");

    shaders["mesh"] = std::make_shared<QOpenGLShaderProgram>();
    shaders["mesh"]->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/meshVertexShader.glsl");
    shaders["mesh"]->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/meshFragmentShader.glsl");

    for (const auto& [type, shader]: shaders) {
        if (!shader->link()) {
            qDebug() << "Shader linking failed: " << shader->log();
        }
    }
}

void OpenGLWidget::createVaos()
{
    vaos["cube"] = std::make_shared<QOpenGLVertexArrayObject>();
    vaos["mesh"] = std::make_shared<QOpenGLVertexArrayObject>();
    for (auto& [type, vao]: vaos) {
        if (!vao->create()) {
            qDebug() << "Vao creating failed: ";
        }
        vao->bind();
    }
}

void OpenGLWidget::setTimer() {
    timer = new QTimer(this);
    timer->setInterval(16);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start();
}

void OpenGLWidget::initializeGL() {
    createShaders();
    createVaos();
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glClearColor(0.7f, 0.7f, 0.7f, 1);
    f->glEnable(GL_DEPTH_TEST);
    f->glEnable(GL_PROGRAM_POINT_SIZE);
    setTimer();
}

void OpenGLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void OpenGLWidget::paintGL()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    updateCamera();

    // if (started) {
    //     if (physicsThread.joinable()) {
    //         physicsThread.join();
    //     }
    //     physicsThread = std::thread(&Physics::calculateIntersect, &physic, std::ref(shapes));
    // }

    if (started) {
        physic.calculateIntersect(shapes);
    }

    for (auto shape : shapes) {
        shape->setViewMatrix(camera.getViewMatrix());
        shape->setProjectionMatrix(camera.getProjectionMatrix((float)width() / (float)height()));
        if (started) {
            shape->update(deltaTime / 1000);
        }
        shape->draw();
    }
    ++frameCount;
}

void OpenGLWidget::keyPressEvent(QKeyEvent *event) {
    
    keys[event->key()] = true;
}

void OpenGLWidget::keyReleaseEvent(QKeyEvent *event) {
    updateCamera();
    keys[event->key()] = false;
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *event) {
    float xOffset = event->position().x() - lastX;
    float yOffset = lastY - event->position().y();

    lastX = event->position().x();
    lastY = event->position().y();

    camera.processMouseMovement(xOffset, yOffset);
}

void OpenGLWidget::wheelEvent(QWheelEvent *event)
{
    float yOffset = event->angleDelta().y() / 120.0f;
    camera.processMouseScroll(yOffset);
    update();
}

void OpenGLWidget::updateCamera() {
    float deltaTime = 0.1f;

    if (keys[Qt::Key_W]) {
        camera.processKeyboard(Qt::Key_W, deltaTime);
    }
    if (keys[Qt::Key_S]) {
        camera.processKeyboard(Qt::Key_S, deltaTime);
    }
    if (keys[Qt::Key_A]) {
        camera.processKeyboard(Qt::Key_A, deltaTime);
    }
    if (keys[Qt::Key_D]) {
        camera.processKeyboard(Qt::Key_D, deltaTime);
    }
}

void OpenGLWidget::setLigths()
{
    for (const auto& shape: shapes) {
        shape->setLights(lights);
    }
}
