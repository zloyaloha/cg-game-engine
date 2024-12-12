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
#include <memory>
#include "camera.h"
#include <QTimer>

struct Light {
    glm::vec3 position;
    glm::vec3 dir;
    glm::vec3 color;
};

class OpenGLWidget : public QOpenGLWidget {
    Q_OBJECT

public:
    OpenGLWidget(QWidget *parent = nullptr);
    void addShape(std::shared_ptr<Shape> shape);
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
private:
    Camera camera;
    bool keys[256];
    void updateCamera();
    std::shared_ptr<QOpenGLShaderProgram> shaderProgram;
    std::vector<Light> lights;
    std::vector<std::shared_ptr<Shape>> shapes;
    Cube *cube;
    QTimer* timer;
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    QOpenGLContext* m_context;
    QOpenGLFunctions_4_3_Core* m_funcs;
    float rotationAngle;
};
