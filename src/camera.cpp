#include "camera.h"
#include <QDebug>
void Camera::processKeyboard(int key, float deltaTime) {
    float velocity = Speed * deltaTime;

    if (key == Qt::Key_W) {
        Position += Front * velocity;
    }
    if (key == Qt::Key_S) {
        Position -= Front * velocity;
    }
    if (key == Qt::Key_A) {
        Position -= glm::normalize(glm::cross(Front, Up)) * velocity;
    }
    if (key == Qt::Key_D) {
        Position += glm::normalize(glm::cross(Front, Up)) * velocity;
    }
}

void Camera::processMouseMovement(float xOffset, float yOffset, bool constrainPitch) {
    xOffset *= Sensitivity;
    yOffset *= Sensitivity;

    Yaw   += xOffset;
    Pitch += yOffset;

    if (constrainPitch) {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

    Front = glm::normalize(front);
}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(Position, (Position + Front), Up);
    // return glm::lookAt(glm::vec3(3.0f, 3.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}