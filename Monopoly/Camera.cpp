#include "Camera.h"

Camera::Camera() {}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
    position = startPosition;
    initialPosition = startPosition;  // Asigna la posici�n inicial
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

void Camera::switchCamera(int& cameraMode, bool* keys)
{
    static bool keyKPressed = false;  // Variable est�tica para detectar cuando se presiona 'K'

    if (keys[GLFW_KEY_K] && !keyKPressed) {
        // Cambiar al siguiente modo de c�mara c�clicamente (0, 1, 2)
        cameraMode = (cameraMode + 1) % 3;  // Se asegura que los valores de cameraMode est�n en 0, 1, o 2

        keyKPressed = true;

        // Imprimir el cambio de c�mara seg�n el valor de cameraMode
        if (cameraMode == 0) {
            printf("Cambiando a c�mara principal (Main Camera)...\n");
        }
        else if (cameraMode == 1) {
            printf("Cambiando a c�mara isom�trica (Iso Camera)...\n");
        }
        else if (cameraMode == 2) {
            printf("Cambiando a c�mara en seguimiento (Follow Camera)...\n");
        }
    }
    else if (!keys[GLFW_KEY_K]) {
        keyKPressed = false;  // Restablecer cuando se suelta la tecla 'K'
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
    front.y = sin(glm::radians(pitch)); // pitch es 0, as� que esto es siempre 0
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

Camera::~Camera() {}
