#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "StatisticsLayer.h"
#include <iostream>
#include "shaderLoader.h"
#include "TextRenderer.h"
#include "Window.h"
#include "FilePaths.h"
#include "renderingUtilities.h"
#include <glm/glm.hpp>
#include "Event.h"
#include "KeyEvent.h"
#include "Application.h"
// Base layer code from Addison Wesley OpenGL Redbook.

StatisticsLayer::StatisticsLayer() {}

void StatisticsLayer::loadRenderData(Window* window, FilePaths* filePaths) {    
    this->window = window;

    // Set up fbo to be rendered to - prevents mismatching fps causing layers to flicker (i.e. not be displayed on some frames).
    setUpFramebuffer(&framebuffer, &renderTexture);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "Framebuffer not complete." << std::endl;
        exit(1);
    }

    // Load fonts. Function automatically checks if layer has been loaded already.
    window->textRenderer.addFace("bitcount", "Bitcount.ttf", filePaths);
    window->textRenderer.addFace("iosevka", "Iosevka.ttf", filePaths);

    std::vector<float> borderVertices = getQuad(glm::vec2(0.0), 1.95, 1.95);
    createVAO(VAOs[quads], borderVertices);
    bindEBO(getQuadEBO());
    numVertices[quads] = 6;
    glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(vPosition);

    ShaderInfo shaders[] = {
        {GL_VERTEX_SHADER, "passthrough.vert"},
        {GL_FRAGMENT_SHADER, "red.frag"},
        {GL_NONE, NULL},
    };

    programs[quads] = loadShaders(shaders, filePaths);

    renderSetupComplete = true;
}

StatisticsLayer::~StatisticsLayer() {
    std::cout << "Deleting base layer resources." << std::endl;
}

void StatisticsLayer::onUpdate(float timestep) {

}

void StatisticsLayer::onEvent(std::shared_ptr<Event> event) {
    std::cout << "EVENT" << std::endl;
    // Close window if I is pressed.
    if (event->type == EventType::KeyEvent) {
        auto keyEvent = std::dynamic_pointer_cast<KeyEvent>(event);
        if (keyEvent->action == GLFW_PRESS) {
            switch(keyEvent->key) {
                case (GLFW_KEY_I): {
                    std::cout << "I pressed, stats window should close." << std::endl;

                    window->getApplication()->removeWindow("Statistics");                    
                    keyEvent->handled = true;
                    break;
                }
                default:
                    break;
            }
        }
    }

}

void StatisticsLayer::onRender(FilePaths* filePaths) {
    // Bind framebuffer - with texture as colour attachement.
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glViewport(0, 0, 1920, 1080);

    // Clear screen to transparent - allowing lower layers to be seen.
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Render statistics border background.
    glUseProgram(programs[quads]);
    glBindVertexArray(VAOs[quads]);
    glDrawElements(GL_TRIANGLES, numVertices[quads], GL_UNSIGNED_INT, 0);

    window->textRenderer.renderText("bitcount", "Application Template", 800, 540, 0.5f, glm::vec3(0, 255, 0), filePaths);
    window->textRenderer.renderText("iosevka", "In Development ...", 800, 520, 0.5f, glm::vec3(0, 255, 0), filePaths);
}

