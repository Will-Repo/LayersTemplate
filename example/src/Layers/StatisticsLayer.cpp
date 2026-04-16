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

void StatisticsLayer::loadRenderData(Window* window, FilePaths* filepaths) {    
    setupLayer(window, filepaths);

    // Load fonts. Function automatically checks if layer has been loaded already.
    window->textRenderer.addFace("bitcount", filepaths->executablePath + "/" + filepaths->fontsPath + "/Bitcount.ttf");
    window->textRenderer.addFace("iosevka", filepaths->executablePath + "/" + filepaths->fontsPath + "/Iosevka.ttf");

    std::vector<float> borderVertices = getQuad(glm::vec2(0.0f), 1.95f, 1.95f, 0.0f, glm::vec3(0.5f));
    createVAO(VAOs[quads], borderVertices);
    bindEBO(getQuadEBO());
    numVertices[quads] = 6;
    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(vPosition);    
    glVertexAttribPointer(vColour, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(vColour);

    ShaderInfo shaders[] = {
        {GL_VERTEX_SHADER, "passthrough.vert", ShaderDataType::Path},
        {GL_FRAGMENT_SHADER, "passthrough.frag", ShaderDataType::Path},
        {GL_NONE, NULL, ShaderDataType::Path},
    };

    std::string path = filepaths->executablePath + "/" + filepaths->shadersPath;
    programs[quads] = loadShaders(shaders, path);

    renderSetupComplete = true;
}

StatisticsLayer::~StatisticsLayer() {
    std::cout << "Deleting base layer resources." << std::endl;
}

void StatisticsLayer::onUpdate(float timestep) {

}

void StatisticsLayer::onEvent(std::shared_ptr<Event> event) {
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

void StatisticsLayer::onRender() {
    // Bind framebuffer - with texture as colour attachement.
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glViewport(0, 0, 1920, 1080);

    // Clear screen to transparent - allowing lower layers to be seen.
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render statistics border background.
    glUseProgram(programs[quads]);
    glBindVertexArray(VAOs[quads]);
    glDrawElements(GL_TRIANGLES, numVertices[quads], GL_UNSIGNED_INT, 0);

    glDisable(GL_DEPTH_TEST);
    window->textRenderer.renderText("bitcount", "Application Template", 800, 540, 0.5f, glm::vec3(0, 255, 0), filepaths);
    window->textRenderer.renderText("iosevka", "In Development ...", 800, 520, 0.5f, glm::vec3(0, 255, 0), filepaths);
    glEnable(GL_DEPTH_TEST);
}

