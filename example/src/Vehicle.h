#include <glm/glm.hpp>

class Vehicle {
    public:
        // Store data that should be the same for all vehicles.
        float maxSpeed = 50.0f;
        float engineForce = 150.0f;
        float gravity = -10; // Acceleration upwards due to gravity.
        glm::vec3 startingPosition = glm::vec3(0.0f, 5.5f, 0.0f);
};
