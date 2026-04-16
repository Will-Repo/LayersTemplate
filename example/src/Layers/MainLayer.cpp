#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "MainLayer.h"
#include <iostream>
#include "shaderLoader.h"
#include "TextRenderer.h"
#include "Window.h"
#include "FilePaths.h"
#include "renderingUtilities.h"
#include "Event.h"
#include "KeyEvent.h"
#include "MousePositionEvent.h"
#include "Layers/StatisticsLayer.h"
#include "Application.h"
#include "Model.h"
// Base layer code from Addison Wesley OpenGL Redbook.

MainLayer::MainLayer() {}

void MainLayer::loadRenderData(Window* window, FilePaths* filepaths) {
    // Set up fbo to be rendered to - prevents mismatching fps causing layers to flicker (i.e. not be displayed on some frames).
    setupLayer(window, filepaths);

    // Load fonts. Function automatically checks if layer has been loaded already.
    window->textRenderer.addFace("bitcount", filepaths->executablePath + "/" + filepaths->fontsPath + "/Bitcount.ttf");
    window->textRenderer.addFace("iosevka", filepaths->executablePath + "/" + filepaths->fontsPath + "/Iosevka.ttf");

    std::vector<float> vertices = {
        -0.9f, -0.9f,  0.0f, 1.0f, 0.0f, 0.0f,
         0.85f, -0.9f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.9f,  0.85f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.9f, -0.85f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.9f,  0.9f,  0.0f, 1.0f, 0.0f, 0.0f,
        -0.85f, 0.9f,  0.0f, 1.0f, 0.0f, 0.0f
    };
    createVAO(VAOs[dualTriangle], vertices);
    numVertices[dualTriangle] = 6;
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
    programs[dualTriangle] = loadShaders(shaders, path);

    // Render world - sphere boundary.
    Model world = Model(filepaths->executablePath + "/" + filepaths->assetsPath + "/sphere.obj");
    models[sphere] = world;
    ShaderInfo mvpShaders[] = {
        {GL_VERTEX_SHADER, "mvp.vert", ShaderDataType::Path},
        {GL_FRAGMENT_SHADER, "mvp.frag", ShaderDataType::Path},
        {GL_NONE, NULL, ShaderDataType::Path},
    };
    modelPrograms[sphere] = loadShaders(mvpShaders, path);    

    Model object = Model(filepaths->executablePath + "/" + filepaths->assetsPath + "/cube.obj");
    models[cube] = object;
    modelPrograms[cube] = loadShaders(mvpShaders, path);    

    // Set MVP matrix initial values.
    mvp.projection = glm::perspective(glm::radians(45.0f), (float)1920 / (float)1080, 0.1f, 100.0f);
    cameraChanged = true;

    glfwSetInputMode(window->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    renderSetupComplete = true;
}

MainLayer::~MainLayer() {
    std::cout << "Deleting base layer resources." << std::endl;
}

void MainLayer::onUpdate(float timestep) {
    if (forwardHeld) {
        camera.position += camera.front * camera.speed * timestep;
        cameraChanged = true;
    }
    if (leftHeld) {
        camera.position -= camera.right * camera.speed * timestep;
        cameraChanged = true;
    }
    if (backwardsHeld) {
        camera.position -= camera.front * camera.speed * timestep;
        cameraChanged = true;
    }
    if (rightHeld) {
        camera.position += camera.right * camera.speed * timestep;
        cameraChanged = true;
    }
    if (downHeld) {
        camera.position -= camera.up * camera.speed * timestep;
        cameraChanged = true;
    }
    if (upHeld) {
        camera.position += camera.up * camera.speed * timestep;
        cameraChanged = true;
    }
    if (cameraSpeedIncreaseHeld) {
        camera.speed += 3.0f * timestep;
    }
    if (cameraSpeedDecreaseHeld) {
        camera.speed -= 3.0f * timestep;
        camera.speed = std::max(camera.speed, 0.0f);
    }
}

void MainLayer::onEvent(std::shared_ptr<Event> event) {
    // Event timing:
        // Movement should be limited by updates, but camera should be independent of this.
    // If event is open statistics key, open statistics window.
    if (event->type == EventType::KeyEvent) {
        auto keyEvent = std::dynamic_pointer_cast<KeyEvent>(event);
        if (keyEvent->action == GLFW_PRESS) {
            switch(keyEvent->key) {
                case (GLFW_KEY_I): {
                    std::cout << "I pressed, stats window should open/close." << std::endl;

                    // If window is already open, close it.
                    if (window->getApplication()->hasWindow("Statistics")) {
                        window->getApplication()->removeWindow("Statistics");                    
                        keyEvent->handled = true;
                        break;
                    }

                    // If window is not open, open it.
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
                // Movement
                case (GLFW_KEY_W): 
                    forwardHeld = true;
                    keyEvent->handled = true;
                    break;
                case (GLFW_KEY_A): 
                    leftHeld = true;
                    keyEvent->handled = true;
                    break;
                case (GLFW_KEY_S): 
                    backwardsHeld = true;
                    keyEvent->handled = true;
                    break;
                case (GLFW_KEY_D):
                    rightHeld = true;
                    keyEvent->handled = true;
                    break;
                case (GLFW_KEY_Q):
                    downHeld = true;
                    keyEvent->handled = true;
                    break;
                case (GLFW_KEY_E):
                    upHeld = true;
                    keyEvent->handled = true;
                    break;
                case (GLFW_KEY_J):
                    cameraSpeedDecreaseHeld = true;
                    keyEvent->handled = true;
                    break;
                case (GLFW_KEY_K):
                    cameraSpeedIncreaseHeld = true;
                    keyEvent->handled = true;
                    break;
                default:
                    break;
            }
        } else if (keyEvent->action == GLFW_RELEASE) {
            switch(keyEvent->key) {
                case (GLFW_KEY_W):
                    forwardHeld = false;
                    keyEvent->handled = true;
                    break; 
                case (GLFW_KEY_A): 
                    leftHeld = false;
                    keyEvent->handled = true;
                    break;
                case (GLFW_KEY_S): 
                    backwardsHeld = false;
                    keyEvent->handled = true;
                    break;
                case (GLFW_KEY_D):
                    rightHeld = false;
                    keyEvent->handled = true;
                    break;
                case (GLFW_KEY_Q):
                    downHeld = false;
                    keyEvent->handled = true;
                    break;
                case (GLFW_KEY_E):
                    upHeld = false;
                    keyEvent->handled = true;
                    break;
                case (GLFW_KEY_J):
                    cameraSpeedDecreaseHeld = false;
                    keyEvent->handled = true;
                    break;
                case (GLFW_KEY_K):
                    cameraSpeedIncreaseHeld = false;
                    keyEvent->handled = true;
                    break;

                default:
                    break;
            }
        }
    }
    // Check for if mouse moved to move camera.
    if (event->type == EventType::MousePositionEvent) {
        auto mousePositionEvent = std::dynamic_pointer_cast<MousePositionEvent>(event);
        if (firstMouse) {
            mouse.x = mousePositionEvent->xpos;
            mouse.y = mousePositionEvent->ypos;
            firstMouse = false;
            return;
        }
        double xOffset = mousePositionEvent->xpos - mouse.x;
        mouse.x = mousePositionEvent->xpos;
        double yOffset = mousePositionEvent->ypos - mouse.y;
        mouse.y = mousePositionEvent->ypos;

        xOffset *= mouse.sensitivity;
        yOffset *= mouse.sensitivity;

        camera.yaw   += xOffset;
        camera.pitch -= yOffset; 

        camera.pitch = std::min(camera.pitch, 89.9f);
        camera.pitch = std::max(camera.pitch, -89.9f);

        cameraChanged = true;

        mousePositionEvent->handled = true; 
    }
}

void MainLayer::onRender() {
    // Bind framebuffer - with texture as colour attachement.
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glViewport(0, 0, 1920, 1080);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render dual triangles background.
    //glUseProgram(programs[dualTriangle]);
    //glBindVertexArray(VAOs[dualTriangle]);
    //glDrawArrays(GL_TRIANGLES, 0, numVertices[dualTriangle]);

    if (cameraChanged) {
        glm::vec3 direction;
        direction.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
        direction.y = sin(glm::radians(camera.pitch));
        direction.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
        camera.front = glm::normalize(direction);
        camera.right = glm::normalize(glm::cross(camera.front, glm::vec3(0.0f, 1.0f, 0.0f)));
        camera.up = glm::normalize(glm::cross(camera.right, camera.front));

        mvp.view = glm::lookAt(camera.position, camera.position + camera.front, camera.up);

        glUseProgram(modelPrograms[sphere]);
        int uniformLoc = glGetUniformLocation(modelPrograms[sphere], "model");
        glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(mvp.model));
        uniformLoc = glGetUniformLocation(modelPrograms[sphere], "view");
        glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(mvp.view));
        uniformLoc = glGetUniformLocation(modelPrograms[sphere], "projection");
        glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(mvp.projection));

        glUseProgram(modelPrograms[cube]);
        uniformLoc = glGetUniformLocation(modelPrograms[cube], "model");
        glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(mvp.model));
        uniformLoc = glGetUniformLocation(modelPrograms[cube], "view");
        glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(mvp.view));
        uniformLoc = glGetUniformLocation(modelPrograms[cube], "projection");
        glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(mvp.projection));

        cameraChanged = false;
    }
    glUseProgram(modelPrograms[sphere]);
    models[sphere].drawModel(modelPrograms[sphere]);
    glUseProgram(modelPrograms[cube]);
    models[cube].drawModel(modelPrograms[cube]);

    //window->textRenderer.renderText("bitcount", "Application Template", 800, 540, 0.5f, glm::vec3(0, 255, 0), filepaths);
    //window->textRenderer.renderText("iosevka", "In Development ...", 800, 520, 0.5f, glm::vec3(0, 255, 0), filepaths);
}

