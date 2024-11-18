#include "FollowCamera.h"

FollowCamera::FollowCamera() {}

FollowCamera::FollowCamera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
    position = startPosition;
    worldUp = startUp;
    yaw = startYaw;
    pitch = startPitch;
    front = glm::vec3(0.0f, 0.0f, -1.0f);

    moveSpeed = startMoveSpeed;
    turnSpeed = startTurnSpeed;

    update();
}

glm::mat4 FollowCamera::calculateViewMatrix()
{
    return glm::lookAt(position, position + front, up);
}

glm::vec3 FollowCamera::getCameraPosition()
{
    return position;
}

glm::vec3 FollowCamera::getCameraDirection()
{
    return glm::normalize(front);
}

void FollowCamera::update()
{
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

FollowCamera::~FollowCamera() {}
