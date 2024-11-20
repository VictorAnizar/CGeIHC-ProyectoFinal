#include "Camera.h"

Camera::Camera() {}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
    position = startPosition;
    initialPosition = startPosition;  // Asigna la posición inicial
    worldUp = startUp;
    yaw = startYaw;
    pitch = startPitch;
    front = glm::vec3(0.0f, 0.0f, -1.0f);


    moveSpeed = startMoveSpeed;
    turnSpeed = startTurnSpeed;

    update();
}

void Camera::keyControl(bool* keys, GLfloat deltaTime)
{

    GLfloat velocity = moveSpeed * deltaTime;

    if (keys[GLFW_KEY_W]) {
        position += velocity;  // Mover hacia arriba
    }

    if (keys[GLFW_KEY_S]) {
        position -= velocity;  // Mover hacia abajo
    }

    if (keys[GLFW_KEY_A]) {
        position -= right * velocity; // Movimiento a la izquierda
    }

    if (keys[GLFW_KEY_D]) {
        position += right * velocity; // Movimiento a la derecha
    }

    // Asegurarse de que la altura permanezca fija
    position.y = initialPosition.y;
}

void Camera::switchCameraMode(int& cameraMode, bool* keys)
{
    static bool keyKPressed = false;

    if (keys[GLFW_KEY_K] && !keyKPressed) {
        cameraMode = (cameraMode + 1) % 3;  // Alternar entre 3 modos de cámara
        keyKPressed = true;

        switch (cameraMode) {
        case 0:
            printf(" [Camara estatica]\n");
            break;
        case 1:
            printf(" [Camara ortogonal]\n");
            break;
        case 2:
            printf(" [Camara en tercera persona]\n");
            break;
        }
    }
    else if (!keys[GLFW_KEY_K]) {
        keyKPressed = false;
    }
}


void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
    xChange *= turnSpeed;
    yChange *= turnSpeed;

    yaw += xChange;

    // Mantener pitch constante para evitar movimiento vertical
    pitch = -90.0f;

    update();
}

glm::mat4 Camera::calculateViewMatrix()
{
    return glm::lookAt(position, position + front, up);
}

glm::vec3 Camera::getCameraPosition()
{
    return position;
}

glm::vec3 Camera::getCameraDirection()
{
    return glm::normalize(front);
}

void Camera::update()
{
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch)); // pitch es 0, así que esto es siempre 0
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

Camera::~Camera() {}
