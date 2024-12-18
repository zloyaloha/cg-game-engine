#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <QOpenGLShaderProgram>
#include <string>

class Light {
    public:
        enum LightType { POINT, SPOT, DIRECTIONAL };
        Light(LightType type, glm::vec3 color = glm::vec3(1.0f), float intensity = 10.0f)
        : type(type), color(color), intensity(intensity) {}

        virtual void loadLightToShader(std::shared_ptr<QOpenGLShaderProgram> shader, int index) const = 0;
        LightType getType() const { return type; }
    public:
        glm::vec3 color;
        float intensity;
    private:
        LightType type;
};

class PointLight : public Light {
    public:
        glm::vec3 position;
        float constant;
        float linear;
        float quadratic;
        PointLight(glm::vec3 position, glm::vec3 color = glm::vec3(1.0f), float intensity = 50.0f,
               float constant = 1.0f, float linear = 0.09f, float quadratic = 0.032f)
        : Light(POINT, color, intensity), position(position), constant(constant), linear(linear), quadratic(quadratic) {}


        void loadLightToShader(std::shared_ptr<QOpenGLShaderProgram> shader, int index) const override;
};

class SpotLight : public Light {
    public:
        glm::vec3 position;
        glm::vec3 direction;
        float cutOff;
        float outerCutOff;

        SpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 color = glm::vec3(1.0f), float intensity = 10.0f,
              float cutOff = glm::cos(glm::radians(12.5f)), float outerCutOff = glm::cos(glm::radians(15.0f)))
        : Light(SPOT, color, intensity), position(position), direction(direction), cutOff(cutOff), outerCutOff(outerCutOff) {}
        
        void loadLightToShader(std::shared_ptr<QOpenGLShaderProgram> shader, int index) const override;
};

class DirectionalLight : public Light {
    public:
        glm::vec3 direction;

        DirectionalLight(glm::vec3 direction, glm::vec3 color = glm::vec3(1.0f), float intensity = 10.0f)
        : Light(DIRECTIONAL, color, intensity), direction(direction) {}

        void loadLightToShader(std::shared_ptr<QOpenGLShaderProgram> shader, int index) const override;
};