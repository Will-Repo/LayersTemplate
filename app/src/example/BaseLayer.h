#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Layer.h"

#pragma once 

class BaseLayer : public Layer {
    private:
        enum VAO_IDs {Triangles, NumVAOs}; //Position in enum is number of VAOs.
        enum Buffer_IDs {ArrayBuffer, NumBuffers};
        enum Attribute_IDs {vPosition = 0};
        GLuint VAOs[NumVAOs];
        GLuint Buffers[NumBuffers];
        const GLuint NumVertices = 6;
        GLuint program;
    public:
        BaseLayer();
        ~BaseLayer();
        void loadData(Window* window) override;
        void onUpdate(float timestep) override;
        void onEvent(Event& event) override;
        void onRender(Window* window) override;
};  
