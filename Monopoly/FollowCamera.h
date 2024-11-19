#pragma once

#include <glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <glfw3.h>

class FollowCamera {
public:
    FollowCamera();
    FollowCamera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);

    void followTarget(const glm::mat4& modelMatrix, GLfloat offsetX, GLfloat offsetY, GLfloat offsetZ, GLfloat dirAvatar);

    glm::vec3 getCameraPosition();
    glm::vec3 getCameraDirection();
    glm::mat4 calculateViewMatrix();

    ~FollowCamera();

private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    GLfloat yaw;
    GLfloat pitch;

    GLfloat moveSpeed;
    GLfloat turnSpeed;

    void update();
};
