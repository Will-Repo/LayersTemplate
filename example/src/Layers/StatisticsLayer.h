#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Layer.h"
#include "FilePaths.h"

#pragma once 

class StatisticsLayer : public Layer {
    public:
        StatisticsLayer();
        ~StatisticsLayer();
        void loadRenderData(Window* window, FilePaths* filePaths) override;
        void onUpdate(float timestep) override;
        void onEvent(std::shared_ptr<Event> event) override;
        void onRender() override;
    private:
        enum VAO_IDs {quads, numVAOs}; //Position in enum is number of VAOs.
        GLuint VAOs[numVAOs];
        GLuint numVertices[numVAOs];
        enum Attribute_IDs {vPosition, vColour}; //Default/common values, can use others if necessary.
        GLuint programs[numVAOs];
};  
