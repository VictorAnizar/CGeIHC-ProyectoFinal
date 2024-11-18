#include "StaticCamera.h"

StaticCamera::StaticCamera()
{
    // Establecer valores predeterminados para la c�mara isom�trica
    position = glm::vec3(10.0f, 30.0f, 10.0f); // Posici�n fija
    worldUp = glm::vec3(0.0f, 0.0f, 1.0f);    // Direcci�n "arriba" (Eje Z)

    // C�mara mirando en una direcci�n diagonal (45 grados)
    GLfloat angle = glm::radians(45.0f); // �ngulo de 45 grados
    front = glm::vec3(-cos(angle), -cos(angle), -sin(angle)); // Direcci�n de c�mara en diagonal

    right = glm::vec3(1.0f, 0.0f, 0.0f);  // Eje X
    up = glm::vec3(0.0f, 0.0f, 1.0f);     // Eje Z (Arriba)
}

StaticCamera::StaticCamera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
    position = startPosition;
    worldUp = startUp;
    front = glm::vec3(0.0f, -1.0f, 0.0f); // C�mara mirando en direcci�n negativa Y
    right = glm::vec3(1.0f, 0.0f, 0.0f);  // Eje X
    up = glm::vec3(0.0f, 0.0f, 1.0f);     // Eje Z
}

glm::mat4 StaticCamera::calculateViewMatrix()
{
    // La c�mara est� fija, pero esta funci�n calcula la vista.
    return glm::lookAt(position, position + front, up);
}

glm::mat4 StaticCamera::calculateProjectionMatrix(GLfloat width, GLfloat height)
{
    // Usamos una proyecci�n ortogr�fica para mantener la c�mara isom�trica.
    GLfloat aspect = width / height;
    GLfloat orthoSize = 10.0f; // Tama�o de la vista ortogr�fica
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
