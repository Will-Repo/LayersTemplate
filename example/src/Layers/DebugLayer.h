#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Layer.h"
#include "FilePaths.h"

#pragma once 

class DebugLayer : public Layer {
    public:
        DebugLayer();
        ~DebugLayer();
        void loadRenderData(Window* window, FilePaths* filePaths) override;
        void onUpdate(float timestep) override;
        void onEvent(std::shared_ptr<Event> event) override;
        void onRender() override; 
    private:
        bool showDebugInfo = false;
};  
