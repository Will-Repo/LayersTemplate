#include "Car.h"

glm::vec3 Car::getDirection() {
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    return glm::normalize(direction);
}

glm::vec3 Car::getRight() {
    return glm::normalize(glm::cross(getDirection(), glm::vec3(0.0f, 1.0f, 0.0f)));
}

glm::vec3 Car::getUp() {
    return glm::normalize(glm::cross(getRight(), getDirection()));
}
