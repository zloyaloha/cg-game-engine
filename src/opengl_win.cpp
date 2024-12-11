#include "opengl_win.h"
#include <iostream>

OpenGLWidget::OpenGLWidget(QWidget *parent) : QOpenGLWidget(parent) {
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(4, 2); // Используйте OpenGL 3.3 Core
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);
}

void OpenGLWidget::initializeGL() {
    projectionMatrix = glm::perspective(glm::radians(45.0f), (float)width() / (float)height(), 0.1f, 100.0f);
    viewMatrix = glm::lookAt(glm::vec3(3.0f, 3.0f, 5.0f),
                             glm::vec3(0.0f, 0.0f, 0.0f),
                             glm::vec3(0.0f, 1.0f, 0.0f));

    cube = new Cube(1.0f, glm::vec3(1.0f, 0.0f, -5.0f));

    cube->initialize();
}

void OpenGLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);  // Устанавливаем область рендеринга
    projectionMatrix = glm::perspective(glm::radians(45.0f), (float)w / (float)h, 0.1f, 100.0f);
}

void OpenGLWidget::paintGL()
{   
    Cube* cube2 = new Cube(1.0f, glm::vec3(-1.0f, 0.0f, -5.0f));
    cube2->initialize();

    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Очистка экрана
    cube->setProjectionMatrix(projectionMatrix);
    cube->setViewMatrix(viewMatrix);
    cube2->setProjectionMatrix(projectionMatrix);
    cube2->setViewMatrix(viewMatrix);

    cube2->draw();
    cube->draw();
}