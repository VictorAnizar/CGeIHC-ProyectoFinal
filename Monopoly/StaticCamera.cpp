#include "StaticCamera.h"

StaticCamera::StaticCamera()
{
    // Establecer valores predeterminados para la cámara isométrica
    position = glm::vec3(10.0f, 30.0f, 10.0f); // Posición fija
    worldUp = glm::vec3(0.0f, 0.0f, 1.0f);    // Dirección "arriba" (Eje Z)

    // Cámara mirando en una dirección diagonal (45 grados)
    GLfloat angle = glm::radians(45.0f); // Ángulo de 45 grados
    front = glm::vec3(-cos(angle), -cos(angle), -sin(angle)); // Dirección de cámara en diagonal

    right = glm::vec3(1.0f, 0.0f, 0.0f);  // Eje X
    up = glm::vec3(0.0f, 0.0f, 1.0f);     // Eje Z (Arriba)
}

StaticCamera::StaticCamera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
    position = startPosition;
    worldUp = startUp;
    front = glm::vec3(0.0f, -1.0f, 0.0f); // Cámara mirando en dirección negativa Y
    right = glm::vec3(1.0f, 0.0f, 0.0f);  // Eje X
    up = glm::vec3(0.0f, 0.0f, 1.0f);     // Eje Z
}

glm::mat4 StaticCamera::calculateViewMatrix()
{
    // La cámara está fija, pero esta función calcula la vista.
    return glm::lookAt(position, position + front, up);
}

glm::mat4 StaticCamera::calculateProjectionMatrix(GLfloat width, GLfloat height)
{
    // Usamos una proyección ortográfica para mantener la cámara isométrica.
    GLfloat aspect = width / height;
    GLfloat orthoSize = 10.0f; // Tamaño de la vista ortográfica
    return glm::ortho(-orthoSize * aspect, orthoSize * aspect, -orthoSize, orthoSize, 0.1f, 100.0f);
}

glm::vec3 StaticCamera::getCameraPosition()
{
    return position;
}

glm::vec3 StaticCamera::getCameraDirection()
{
    return glm::normalize(front);
}

StaticCamera::~StaticCamera() {}
