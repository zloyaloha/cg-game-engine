#include "opengl_win.h"
#include <iostream>

OpenGLWidget::OpenGLWidget(QWidget *parent) : QOpenGLWidget(parent),
    camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f))
{
    setFocusPolicy(Qt::StrongFocus);
    this->setFocus();
    std::fill(std::begin(keys), std::end(keys), false);
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(4, 2); // Используйте OpenGL 3.3 Core
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

}

void OpenGLWidget::addShape(std::shared_ptr<Shape> shape)
{
    shape->setProjectionMatrix(projectionMatrix);
    shape->setViewMatrix(camera.getViewMatrix());
    shape->setShader(shaders[shape->getType()]);
    shape->initialize();
    shapes.push_back(shape);
    setLigths();
    update();
}

void OpenGLWidget::addLight(std::shared_ptr<Light> light)
{
    lights.push_back(light);
    setLigths();
}

void OpenGLWidget::createShaders() {
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

void OpenGLWidget::setTimer() {
    timer = new QTimer(this);
    timer->setInterval(32);  // Интервал 16 мс (примерно 60 кадров в секунду)
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start();
    qDebug() << "Timer started";
}

void OpenGLWidget::initializeGL() {
    createShaders();
    projectionMatrix = glm::perspective(glm::radians(45.0f), (float)width() / (float)height(), 0.1f, 100.0f);
    setTimer();
}

void OpenGLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    projectionMatrix = glm::perspective(glm::radians(45.0f), (float)w / (float)h, 0.1f, 100.0f);
}

void OpenGLWidget::paintGL()
{   
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    updateCamera();
    for (auto shape: shapes) {
        shape->setViewMatrix(camera.getViewMatrix());
        shape->initialize();
        shape->draw();
    }
}

void OpenGLWidget::keyPressEvent(QKeyEvent *event) {
    keys[event->key()] = true;
}

void OpenGLWidget::keyReleaseEvent(QKeyEvent *event) {
    updateCamera();
    keys[event->key()] = false;
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *event) {
    static float lastX = width() / 2.0f;
    static float lastY = height() / 2.0f;

    float xOffset = event->x() - lastX;
    float yOffset = lastY - event->y();

    lastX = event->x();
    lastY = event->y();

    camera.processMouseMovement(xOffset, yOffset);
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
