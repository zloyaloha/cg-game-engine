#pragma once
#include "cube.h"
#include "mesh.h"
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
#include <QDialog>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <iostream>

class OpenGLWidget : public QOpenGLWidget {
    Q_OBJECT

public:
    OpenGLWidget(QWidget *parent = nullptr);
    void addShape(std::shared_ptr<Shape> shape);
    void addLight(std::shared_ptr<Light> light);
    std::vector<std::shared_ptr<Shape>> getAllShapes();
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void createShaders();
    void setTimer();
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
private:
    Camera camera;
    bool keys[256];
    Cube *cube;
    QTimer* timer;
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    QOpenGLContext* m_context;
    QOpenGLFunctions_4_3_Core* m_funcs;
    float rotationAngle;
    float aspectRatio;
    float lastX{0}, lastY{0};

    std::vector<std::shared_ptr<Light>> lights;
    std::vector<std::shared_ptr<Shape>> shapes;
    std::unordered_map<std::string, std::shared_ptr<QOpenGLShaderProgram>> shaders;
private:
    void updateCamera();
    void setLigths();
};
