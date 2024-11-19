#pragma once

#include <glew.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <glfw3.h>

class StaticCamera
{
public:
    // Constructor predeterminado
    StaticCamera();



    // Constructor con parámetros
    StaticCamera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startMoveSpeed, GLfloat startTurnSpeed);

    glm::mat4 calculateViewMatrix();
    glm::mat4 calculateProjectionMatrix(GLfloat width, GLfloat height);

    glm::vec3 getCameraPosition();
    glm::vec3 getCameraDirection();

    ~StaticCamera();

private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    void update();
    void keyControl(bool* keys, GLfloat deltaTime);
};
