#pragma once 
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Layer.h"
#include "FilePaths.h"
#include "Model.h"

class MainLayer : public Layer {
    public:
        MainLayer();
        ~MainLayer();
        void loadRenderData(Window* window, FilePaths* filePaths) override;
        void onUpdate(float timestep) override;
        void onEvent(std::shared_ptr<Event> event) override;
        void onRender(FilePaths* filePaths) override;
    private:
        enum VAO_IDs {dualTriangle, numVAOs}; //Position in enum is number of VAOs.
        GLuint VAOs[numVAOs];
        GLuint numVertices[numVAOs];
        enum Attribute_IDs {vPosition, vColour}; //Default/common values, can use others if necessary.
        GLuint programs[numVAOs];

        enum Model_IDS {sphere, numModels};
        Model models[numModels];
        unsigned int modelPrograms[numModels];
};  
