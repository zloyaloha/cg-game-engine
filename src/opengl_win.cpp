#include "opengl_win.h"
#include <iostream>

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
}

void OpenGLWidget::addShape(std::shared_ptr<Shape> shape)
{
    shape->setProjectionMatrix(camera.getProjectionMatrix(aspectRatio));
    shape->setViewMatrix(camera.getViewMatrix());
    shape->setShader(shaders[shape->getType()]);
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
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
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
    for (auto shape: shapes) {
        shape->setViewMatrix(camera.getViewMatrix());
        shape->setProjectionMatrix(camera.getProjectionMatrix((float)width() / (float)height()));
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
