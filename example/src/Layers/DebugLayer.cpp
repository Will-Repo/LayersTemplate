#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "DebugLayer.h"
#include <iostream>
#include "shaderLoader.h"
#include "FilePaths.h"
#include "renderingUtilities.h"
#include "Event.h"
#include "KeyEvent.h"
#include "Window.h"

DebugLayer::DebugLayer() {}

void DebugLayer::loadRenderData(Window* window, FilePaths* filePaths) {
    this->window = window;

    setUpFramebuffer(&framebuffer, &renderTexture);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "Framebuffer not complete." << std::endl;
        exit(1);
    }

    window->textRenderer.addFace("bitcount", "Bitcount.ttf", filePaths);

    glGenVertexArrays(NumVAOs, VAOs);
    glBindVertexArray(VAOs[Triangles]);

    GLfloat vertices[NumVertices][2] = {
        {-0.9, -0.9},
        {0.85, -0.9},
        {-0.9, 0.85},
        {0.9, -0.85},
        {0.9, 0.9},
        {-0.85, 0.9},

        {-0.5, -0.5},
        {0.5, -0.5},
        {-0.5, 0.5},
        {0.5, -0.5},
        {0.5, 0.5},
        {-0.5, 0.5}
    };

    glGenBuffers(NumBuffers, Buffers);
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    ShaderInfo shaders[] = {
        {GL_VERTEX_SHADER, "passthrough.vert"},
        {GL_FRAGMENT_SHADER, "red.frag"},
        {GL_NONE, NULL},
    };

    program = loadShaders(shaders, filePaths);

    glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(vPosition);
    
    renderSetupComplete = true;
}

DebugLayer::~DebugLayer() {
    std::cout << "Deleting base layer resources." << std::endl;
}

void DebugLayer::onUpdate(float timestep) {

}

void DebugLayer::onEvent(std::shared_ptr<Event> event) {
    // If event is open statistics key, open statistics window.
    if (event->type == EventType::KeyEvent) {
        auto keyEvent = std::dynamic_pointer_cast<KeyEvent>(event);
        if (keyEvent->action == GLFW_PRESS) {
            switch(keyEvent->key) {
                case (GLFW_KEY_O): {
                    std::cout << "O pressed, debug info should show." << std::endl;
                    showDebugInfo ^= true; // flip value
                    keyEvent->handled = true;
                    break;
                }
                default:
                    break;
            }
        }
    }
}

void DebugLayer::onRender(FilePaths* filePaths) {
    glUseProgram(program);

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glViewport(0, 0, 1920, 1080);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    //glBindVertexArray(VAOs[Triangles]);
    //glDrawArrays(GL_TRIANGLES, 0, NumVertices); 
    if (showDebugInfo) {
        window->textRenderer.renderText("bitcount", "Debug Info", 10, 10, 0.5f, glm::vec3(0, 255, 0), filePaths);
    }
}
