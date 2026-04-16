#pragma once 
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Layer.h"
#include "FilePaths.h"
#include "Model.h"

struct Camera {
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 2.0f);
    glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 direction = glm::normalize(target - position);

    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); 
    glm::vec3 right = glm::normalize(glm::cross(up, direction));
    float speed = 1.0f;
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

        enum Model_IDS {sphere, numModels};
        Model models[numModels];
        unsigned int modelPrograms[numModels];

        MVP mvp;
        Camera camera;
        bool cameraChanged = false;
};  
