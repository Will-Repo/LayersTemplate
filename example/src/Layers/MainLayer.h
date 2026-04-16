#pragma once 
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Layer.h"
#include "FilePaths.h"
#include "Model.h"

struct Camera {
    glm::vec3 position = glm::vec3(0.0f, 0.4f, 2.0f);
    glm::vec3 target = glm::vec3(0.0f, 0.4f, 0.0f);

    // Direction camera is facing - -z is in front of camera so backwards.
    glm::vec3 front = glm::normalize(position - target);
    glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);//glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), front));
    glm::vec3 up = glm::cross(front, right);
    float speed = 3.0f;
};

struct MVP {
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
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
        enum VAO_IDs {dualTriangle, numVAOs}; //Position in enum is number of VAOs.
        GLuint VAOs[numVAOs];
        GLuint numVertices[numVAOs];
        enum Attribute_IDs {vPosition, vColour}; //Default/common values, can use others if necessary.
        GLuint programs[numVAOs];

        enum Model_IDS {sphere, cube, numModels};
        Model models[numModels];
        unsigned int modelPrograms[numModels];

        MVP mvp;
        Camera camera;
        bool cameraChanged = false;

        bool forwardHeld = false, leftHeld = false, backwardsHeld = false, rightHeld = false, downHeld = false, upHeld = false;
        bool cameraSpeedIncreaseHeld = false, cameraSpeedDecreaseHeld = false;
};  
