#include <glm/glm.hpp>

class Car {
    // Apply forces at COM to keep car upright. 
    public:
        Car() = default;

        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

        glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 angularVelocity = glm::vec3(0.0f, 0.0f, 0.0f);

        float yaw = -90.0f;
        float pitch = 0.0f;
        float roll = 0.0f;

        //TODO: Add acceleration and stuff for forward speed using forces, rather than constant acceleration.
        float speed = 3.0f;
        glm::vec3 getDirection();
    private:
};
