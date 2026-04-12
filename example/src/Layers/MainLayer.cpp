#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "MainLayer.h"
#include <iostream>
#include "shaderLoader.h"
#include "TextRenderer.h"
#include "Window.h"
#include "FilePaths.h"
#include "renderingUtilities.h"
#include "Event.h"
#include "KeyEvent.h"
#include "Layers/StatisticsLayer.h"
#include "Application.h"
// Base layer code from Addison Wesley OpenGL Redbook.

MainLayer::MainLayer() {}

void MainLayer::loadRenderData(Window* window, FilePaths* filePaths) {
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

    std::vector<float> vertices = {
        -0.9, -0.9,
        0.85, -0.9,
        -0.9, 0.85,
        0.9, -0.85,
        0.9, 0.9,
        -0.85, 0.9
    };
    createVAO(VAOs[dualTriangle], vertices);
    numVertices[dualTriangle] = 6;
    glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(vPosition);

    ShaderInfo shaders[] = {
        {GL_VERTEX_SHADER, "passthrough.vert"},
        {GL_FRAGMENT_SHADER, "red.frag"},
        {GL_NONE, NULL},
    };

    programs[dualTriangle] = loadShaders(shaders, filePaths);

    renderSetupComplete = true;
}

MainLayer::~MainLayer() {
    std::cout << "Deleting base layer resources." << std::endl;
}

void MainLayer::onUpdate(float timestep) {

}

void MainLayer::onEvent(std::shared_ptr<Event> event) {
    // If event is open statistics key, open statistics window.
    if (event->type == EventType::KeyEvent) {
        auto keyEvent = std::dynamic_pointer_cast<KeyEvent>(event);
        if (keyEvent->action == GLFW_PRESS) {
            switch(keyEvent->key) {
                case (GLFW_KEY_I): {
                    std::cout << "I pressed, stats window should open." << std::endl;

                    /* WINDOW 2 SETUP */
                    auto window2 = std::make_shared<Window>();    
                    struct Window::Configuration* win2conf = &window2->config;
                    win2conf->windowName = "Statistics";
                    win2conf->windowDesc = "Basic statistics screen";
                    win2conf->running = true;
                    //win2conf->inputHandlingRate = 0;
                    win2conf->inputHandlingRate = 60; //TODO: CHAnge back, to 0, but make 0 work
                    win2conf->inputHandlingGroup = 1; // All interactions go through window 1, so no need for inputs.
                    win2conf->renderingFrameLimit = 60;
                    win2conf->renderingThreadGroup = 1;
                    // Others remain as default. Same as window 1 values.

                    // Add layers to window 2
                    auto stats = std::make_shared<StatisticsLayer>();
                    stats->config.updateFrameLimit = 5; // layer logic framerate.
                    stats->config.renderingFrameLimit = 5;
                    window2->addLayer(std::move(stats));

                    window->getApplication()->addWindow(window2);                    
                    keyEvent->handled = true;
                    break;
                }
                default:
                    break;
            }
        }
    }
}

void MainLayer::onRender(FilePaths* filePaths) {
    // Bind framebuffer - with texture as colour attachement.
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glViewport(0, 0, 1920, 1080);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Render dual triangles background.
    glUseProgram(programs[dualTriangle]);
    glBindVertexArray(VAOs[dualTriangle]);
    glDrawArrays(GL_TRIANGLES, 0, numVertices[dualTriangle]);

    window->textRenderer.renderText("bitcount", "Application Template", 800, 540, 0.5f, glm::vec3(0, 255, 0), filePaths);
    window->textRenderer.renderText("iosevka", "In Development ...", 800, 520, 0.5f, glm::vec3(0, 255, 0), filePaths);
}

