#pragma once 
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Layer.h"
#include "FilePaths.h"
#include "Model.h"
#include "Car.h"

struct Camera {
    glm::vec3 initialPosition = glm::vec3(0.0f, 0.4f, 2.0f);
    glm::vec3 position = initialPosition;

    glm::vec3 front;
    glm::vec3 right;
    glm::vec3 up;

    float speed = 3.0f;

    float yaw = -90.0f;
    float pitch = 0.0f;
    //float roll;
};

struct MVP {
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
};

struct Mouse {
    double x = 0;
    double y = 0;
    float sensitivity = 0.1f;
};

class MainLayer : public Layer {
    public:
        MainLayer();
        ~MainLayer();
        void loadRenderData(Window* window, FilePaths* filePaths) override;
        void onUpdate(float timestep) override;
        void onEvent(std::shared_ptr<Event> event) override;
        void onRender() override;
    private:
        enum VAO_IDs {numVAOs}; //Position in enum is number of VAOs.
        GLuint VAOs[numVAOs];
        GLuint numVertices[numVAOs];
        enum Attribute_IDs {vPosition, vColour}; //Default/common values, can use others if necessary.
        GLuint programs[numVAOs];

        enum Model_IDS {sphere, cube, numModels};
        Model models[numModels];
        unsigned int modelPrograms[numModels];

        MVP mvp;
        Camera camera;
        bool cameraAttached = true; // If camera is attached to car movement.

        bool cameraForwardsHeld = false, cameraLeftHeld = false, cameraBackwardsHeld = false, cameraRightHeld = false, cameraDownHeld = false, cameraUpHeld = false;
        bool cameraSpeedIncreaseHeld = false, cameraSpeedDecreaseHeld = false;

        Mouse mouse;
        bool firstMouse = true;

        Car car;
        bool carForwardsHeld = false, carLeftHeld = false, carBackwardsHeld = false, carRightHeld = false;
};  
