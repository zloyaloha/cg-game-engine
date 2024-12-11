#pragma once
#include "cube.h"
#include <QOpenGLWidget>
#include <QOpenGLContext>
#include <QOpenGLFunctions_4_3_Core>
#include <QOpenGLShaderProgram>
#include <QTimer>
#include <QMatrix4x4>
#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <QOpenGLVertexArrayObject>

class OpenGLWidget : public QOpenGLWidget {
    Q_OBJECT

public:
    OpenGLWidget(QWidget *parent = nullptr);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    QOpenGLShaderProgram shaderProgram;
    QOpenGLBuffer vertexBuffer;
    QOpenGLBuffer indexBuffer;
    QOpenGLVertexArrayObject vao;
    Cube *cube;
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    QOpenGLContext* m_context;
    QOpenGLFunctions_4_3_Core* m_funcs;
    float rotationAngle;
};
