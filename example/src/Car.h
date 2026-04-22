#include <glm/glm.hpp>
#include "Vehicle.h"

class Car : public Vehicle {
    // Apply forces at COM to keep car upright. 
    public:
        Car() = default;

        // Currently 0.1 length cube.
        float length = 0.6f;
        float width = 0.42f;
        float height = 0.3f;

        float steeringAngle = 0.5f * length / width;
        float mass = 100;
    private:
};


        /*glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

        glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 angularVelocity = glm::vec3(0.0f, 0.0f, 0.0f);

        float yaw = -90.0f;
        float pitch = 0.0f;
        float roll = 0.0f;

glm::vec3 getDirection();
        glm::vec3 getRight();
        glm::vec3 getUp();

*/
