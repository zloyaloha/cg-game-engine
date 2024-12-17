#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <QKeyEvent>

class Camera {
public:
    Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up, float near = 0.1f, float far = 100.0f)
        : Position(position), Front(front), Up(up), Speed(0.5f), Sensitivity(0.1f), FOV(45.0f), Near(near), Far(far) {}

    void processKeyboard(int key, float deltaTime);
    void processMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);

    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix(float aspectRatio);

    void setPosition(glm::vec3 position) { Position = position; }
    void setFront(glm::vec3 front) { Front = front; }
    void setUp(glm::vec3 up) { Up = up; }
    void processMouseScroll(float yOffset);

    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    float FOV;
    float Near;
    float Far;
private:
    float Speed;
    float Sensitivity;
    float Yaw = -90.0f;
    float Pitch = 0.0f;
};