#include <glm/glm.hpp>

class Vehicle {
    public:
        // Store data that should be the same for all vehicles.
        float maxSpeed = 20.0f;
        float engineForce = 5.0f;
        float gravity = -10; // Acceleration upwards due to gravity.
        glm::vec3 startingPosition = glm::vec3(0.0f);
};
