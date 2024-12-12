#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <QKeyEvent>

class Camera {
public:
    Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up)
        : Position(position), Front(front), Up(up), Speed(0.05f), Sensitivity(0.1f) {}

    void processKeyboard(int key, float deltaTime);
    void processMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);
    glm::mat4 getViewMatrix();

    void setPosition(glm::vec3 position) { Position = position; }
    void setFront(glm::vec3 front) { Front = front; }
    void setUp(glm::vec3 up) { Up = up; }

    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;

private:
    float Speed;
    float Sensitivity;
    float Yaw = -90.0f;
    float Pitch = 0.0f;
};