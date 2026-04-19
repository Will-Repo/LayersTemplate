#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Layer.h"
#include "FilePaths.h"
#include "MainLayer.h" 
#include <memory>

#pragma once 

class DebugLayer : public Layer {
    public:
        DebugLayer();
        ~DebugLayer();
        void loadData(Window* window, FilePaths* filePaths) override;
        void onUpdate(float timestep) override;
        void onEvent(std::shared_ptr<Event> event) override;
        void onRender() override; 
    private:
        bool showDebugInfo = false;
        std::weak_ptr<MainLayer> game;
        DebugInfo gameSnapshot;
};  
