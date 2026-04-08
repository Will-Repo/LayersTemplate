#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Layer.h"
#include "FilePaths.h"

#pragma once 

class MainLayer : public Layer {
    private:
        enum VAO_IDs {Triangles, NumVAOs}; //Position in enum is number of VAOs.
        enum Buffer_IDs {ArrayBuffer, NumBuffers};
        enum Attribute_IDs {vPosition = 0};
        GLuint VAOs[NumVAOs];
        GLuint Buffers[NumBuffers];
        const GLuint NumVertices = 6;
        GLuint program;
        //GLuint framebuffer; //Provided by layer.
        //GLuint renderTexture;
    public:
        MainLayer();
        ~MainLayer();
        void loadRenderData(Window* window, FilePaths* filePaths) override;
        void onUpdate(float timestep) override;
        void onEvent(std::shared_ptr<Event> event) override;
        void onRender(FilePaths* filePaths) override;
};  
