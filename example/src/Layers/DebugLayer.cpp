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
#include "MainLayer.h"
#include <memory>

DebugLayer::DebugLayer() {}

void DebugLayer::loadData(Window* window, FilePaths* filepaths) {
    setupLayer(window, filepaths);

    setUpFramebuffer(&framebuffer, &renderTexture);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "Framebuffer not complete." << std::endl;
        exit(1);
    }

    window->textRenderer.addFace("bitcount", filepaths->executablePath + "/" + filepaths->fontsPath + "/Bitcount.ttf");

    std::shared_ptr<Layer> layer = window->getLayer("game").lock();
    game = std::dynamic_pointer_cast<MainLayer>(layer);
    if (auto gamePtr = game.lock()) {
        gameSnapshot = gamePtr->getDebugInfo();
    }

    renderSetupComplete = true;
}

DebugLayer::~DebugLayer() {
    std::cout << "Deleting base layer resources." << std::endl;
}

void DebugLayer::onUpdate(float timestep) {
    if (auto gamePtr = game.lock()) {
        gameSnapshot = gamePtr->getDebugInfo();
    }
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

void DebugLayer::onRender() {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glViewport(0, 0, 1920, 1080);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (showDebugInfo) {
        std::string fps = "FPS: " + std::to_string((int)gameSnapshot.fps);
        std::string position = "Position: " + std::to_string((float)gameSnapshot.position.x) + ", " + std::to_string((float)gameSnapshot.position.y) + ", " + std::to_string((float)gameSnapshot.position.z);
        std::string rotation = "Rotation: " + std::to_string((float)gameSnapshot.rotation.x) + ", " + std::to_string((float)gameSnapshot.rotation.y) + ", " + std::to_string((float)gameSnapshot.rotation.z);

        glDisable(GL_DEPTH_TEST);
        window->textRenderer.renderText("bitcount", "Debug Info", 10, 1050, 0.5f, glm::vec3(255, 0, 0), filepaths);
        window->textRenderer.renderText("bitcount", fps, 10, 1020, 0.5f, glm::vec3(255, 0, 0), filepaths);
        window->textRenderer.renderText("bitcount", position, 10, 990, 0.5f, glm::vec3(255, 0, 0), filepaths);
        window->textRenderer.renderText("bitcount", rotation, 10, 960, 0.5f, glm::vec3(255, 0, 0), filepaths);
        glEnable(GL_DEPTH_TEST);
    }
}
