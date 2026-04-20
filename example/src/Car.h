#include <glm/glm.hpp>
#include "Vehicle.h"

class Car : public Vehicle {
    // Apply forces at COM to keep car upright. 
    public:
        Car() = default;

        // Currently 0.1 length cube.
        float length = 1.0f;
        float width = 0.7f;
        float height = 0.5f;

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
