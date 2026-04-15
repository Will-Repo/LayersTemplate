#include "Window.h"
#include "FilePaths.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <renderingUtilities.h>
#include <iostream>

void Layer::setupLayer(Window* window, FilePaths* filepaths) {
    this->window = window;
    this->filepaths = filepaths;
    
    // Set up framebuffer
    setUpFramebuffer(&framebuffer, &renderTexture);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "A framebuffer is not complete for window " << window->config.windowName << "." << std::endl;
        exit(1);
    }
}
