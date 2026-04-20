#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>
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
#include <mutex>
// Base layer code from Addison Wesley OpenGL Redbook.

MainLayer::MainLayer() {}
            
DebugInfo MainLayer::getDebugInfo() {
    std::lock_guard<std::mutex> lock(debugMutex); 
    return debugInfo;
}

void MainLayer::createDebugSnapshot() {
    std::lock_guard<std::mutex> lock(debugMutex); 
    debugInfo.fps = 1.0f / timestep;
}

void MainLayer::loadData(Window* window, FilePaths* filepaths) {
    // Set up fbo to be rendered to - prevents mismatching fps causing layers to flicker (i.e. not be displayed on some frames).
    setupLayer(window, filepaths);

    // Render world - sphere boundary.
    ShaderInfo mvpShaders[] = {
        {GL_VERTEX_SHADER, "mvp.vert", ShaderDataType::Path},
        {GL_FRAGMENT_SHADER, "mvp.frag", ShaderDataType::Path},
        {GL_NONE, NULL, ShaderDataType::Path},
    };    
    std::string path = filepaths->executablePath + "/" + filepaths->shadersPath;
    modelPrograms[sphere] = loadShaders(mvpShaders, path);    
    glUseProgram(modelPrograms[sphere]);
    Model world = Model(filepaths->executablePath + "/" + filepaths->assetsPath + "/world/world.obj");
    models[sphere] = world;
    
    modelPrograms[cube] = loadShaders(mvpShaders, path);    
    glUseProgram(modelPrograms[cube]);
    Model object = Model(filepaths->executablePath + "/" + filepaths->assetsPath + "/cars/default/car.obj");
    models[cube] = object;

    // Set MVP matrix initial values.
    mvp.projection = glm::perspective(glm::radians(45.0f), (float)1920 / (float)1080, 0.1f, 100.0f);

    glfwSetInputMode(window->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    renderSetupComplete = true;
}

MainLayer::~MainLayer() {
    std::cout << "Deleting base layer resources." << std::endl;
}

void MainLayer::onUpdate(float timestep) {
    // Car
    if (carForwardsHeld) {
        //TODO: Need some check that all wheels are on the ground.
        glm::vec3 direction = car.getDirection();
        glm::vec3 movement = glm::normalize(direction) * car.speed * timestep;
        car.velocity += movement;
    }
    if (carLeftHeld) {
        car.yaw -= car.turnSpeed * timestep;
        //TODO: Add max car yaw for turning.
    }
    if (carBackwardsHeld) {
        glm::vec3 direction = car.getDirection();
        glm::vec3 movement = glm::normalize(direction) * car.speed * timestep;
        car.velocity -= movement;
    }
    if (carRightHeld) {
        car.yaw += car.turnSpeed * timestep;
    }

    // Apply friction - temp system.float friction = 5.0f;
    float friction = 1.0f;
    car.velocity *= expf(-friction * timestep);

    car.position += car.velocity * timestep;
    
    // Camera
    if (!cameraAttached) {
        if (cameraForwardsHeld) {
            camera.position += camera.front * camera.speed * timestep;
        }
        if (cameraLeftHeld) {
            camera.position -= camera.right * camera.speed * timestep;
        }
        if (cameraBackwardsHeld) {
            camera.position -= camera.front * camera.speed * timestep;
        }
        if (cameraRightHeld) {
            camera.position += camera.right * camera.speed * timestep;
        }
        if (cameraDownHeld) {
            camera.position -= camera.up * camera.speed * timestep;
        }
        if (cameraUpHeld) {
            camera.position += camera.up * camera.speed * timestep;
        }
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
                    stats->config.name = "statistics";
                    stats->config.updateFrameLimit = 5; // layer logic framerate.
                    stats->config.renderingFrameLimit = 5;
                    window2->addLayer(std::move(stats));

                    window->getApplication()->addWindow(window2);                    
                    keyEvent->handled = true;
                    break;
                }

                // Car Movement
                case (GLFW_KEY_W): 
                    carForwardsHeld = true;
                    keyEvent->handled = true;
                    break;
                case (GLFW_KEY_A): 
                    carLeftHeld = true;
                    keyEvent->handled = true;
                    break;
                case (GLFW_KEY_S): 
                    carBackwardsHeld = true;
                    keyEvent->handled = true;
                    break;
                case (GLFW_KEY_D): 
                    carRightHeld = true;
                    keyEvent->handled = true;
                    break;


                // Camera Movement
                case (GLFW_KEY_UP): 
                    cameraForwardsHeld = true;
                    keyEvent->handled = true;
                    break;
                case (GLFW_KEY_LEFT): 
                    cameraLeftHeld = true;
                    keyEvent->handled = true;
                    break;
                case (GLFW_KEY_DOWN): 
                    cameraBackwardsHeld = true;
                    keyEvent->handled = true;
                    break;
                case (GLFW_KEY_RIGHT):
                    cameraRightHeld = true;
                    keyEvent->handled = true;
                    break;
                case (GLFW_KEY_PAGE_UP):
                    cameraDownHeld = true;
                    keyEvent->handled = true;
                    break;
                case (GLFW_KEY_PAGE_DOWN):
                    cameraUpHeld = true;
                    keyEvent->handled = true;
                    break;
                case (GLFW_KEY_TAB):
                    camera.yaw = -camera.yaw;
                    keyEvent->handled = true;
                    break;

                // Camera settings
                case (GLFW_KEY_J):
                    cameraSpeedDecreaseHeld = true;
                    keyEvent->handled = true;
                    break;
                case (GLFW_KEY_K):
                    cameraSpeedIncreaseHeld = true;
                    keyEvent->handled = true;
                    break;
                case (GLFW_KEY_L):
                    // Reset camera to car.
                    cameraAttached = !cameraAttached;
                    /*if (!cameraAttached) {
                        //TODO: Need to add default offset.
                        camera.position = car.position + camera.initialPosition;
                        camera.yaw = car.yaw;
                        camera.pitch = car.pitch;
                        cameraAttached = true;
                    } else {
                        cameraAttached = false;
                    }*/
                    keyEvent->handled = true;
                    break;

                default:
                    break;
            }
        } else if (keyEvent->action == GLFW_RELEASE) {
            switch(keyEvent->key) {
                // Car Movement
                case (GLFW_KEY_W): 
                    carForwardsHeld = false;
                    keyEvent->handled = true;
                    break;
                case (GLFW_KEY_A): 
                    carLeftHeld = false;
                    keyEvent->handled = true;
                    break;
                case (GLFW_KEY_S): 
                    carBackwardsHeld = false;
                    keyEvent->handled = true;
                    break;
                case (GLFW_KEY_D): 
                    carRightHeld = false;
                    keyEvent->handled = true;
                    break;

                // Camera Movement
                case (GLFW_KEY_UP): 
                    cameraForwardsHeld = false;
                    keyEvent->handled = true;
                    break;
                case (GLFW_KEY_LEFT): 
                    cameraLeftHeld = false;
                    keyEvent->handled = true;
                    break;
                case (GLFW_KEY_DOWN): 
                    cameraBackwardsHeld = false;
                    keyEvent->handled = true;
                    break;
                case (GLFW_KEY_RIGHT):
                    cameraRightHeld = false;
                    keyEvent->handled = true;
                    break;
                case (GLFW_KEY_PAGE_UP):
                    cameraDownHeld = false;
                    keyEvent->handled = true;
                    break;
                case (GLFW_KEY_PAGE_DOWN):
                    cameraUpHeld = false;
                    keyEvent->handled = true;
                    break;
                case (GLFW_KEY_TAB):
                    camera.yaw = -camera.yaw;
                    keyEvent->handled = true;
                    break;


                // Camera settings
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

        mousePositionEvent->handled = true; 
    }
}

void MainLayer::onRender() {
    // Set debug data.
    std::chrono::time_point<std::chrono::high_resolution_clock> now = std::chrono::high_resolution_clock::now();
    timestep = std::chrono::duration<float>(now - lastUpdated).count();
    lastUpdated = now;

    // Bind framebuffer - with texture as colour attachement.
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glViewport(0, 0, 1920, 1080);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // If camera attached, use camera view. Else use fixed car view.
    if (!cameraAttached) {
        glm::vec3 direction;
        direction.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
        direction.y = sin(glm::radians(camera.pitch));
        direction.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
        camera.front = glm::normalize(direction);
        camera.right = glm::normalize(glm::cross(camera.front, glm::vec3(0.0f, 1.0f, 0.0f)));
        camera.up = glm::normalize(glm::cross(camera.right, camera.front));
        mvp.view = glm::lookAt(camera.position, camera.position + camera.front, camera.up);
    } else {
        // Get normalised direction car is facing (ignoring pitch and roll).
        glm::vec3 direction;
        direction.x = cos(glm::radians(car.yaw));// * cos(glm::radians(car.pitch));
        direction.y = 0.0f; //sin(glm::radians(car.pitch));
        direction.z = sin(glm::radians(car.yaw));// * cos(glm::radians(car.pitch));
        glm::vec3 front = glm::normalize(direction);
        glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
        glm::vec3 up = glm::normalize(glm::cross(right, front));

        // Calculate rotation matrix to rotate camera about the car.
        glm::mat4 rotation = glm::yawPitchRoll(
            glm::radians(camera.yaw),
            glm::radians(camera.pitch),
            0.0f
        );

        // Create and rotate base offset - direction from camera to car.
        glm::vec3 offset = glm::vec3(rotation * glm::vec4(0, 0, 1, 0));

        // Rotate the camera relative to the car.
        glm::mat4 carRotation = glm::rotate(
            glm::mat4(1.0f),
            glm::radians(180 - car.yaw),
            glm::vec3(0, 1, 0)
        );
        glm::vec3 finalOffset = glm::vec3(carRotation * glm::vec4(offset, 0.0f));  


        // Update view matrix to point from behind and above car, to car (accounting for mouse camera movement).
        glm::vec3 cameraPosition = car.position - finalOffset * car.length * 5.0f + glm::vec3(0.0f, car.height * 2.5f, 0.0f);

        // Look from new camera position to car.
        //mvp.view = glm::lookAt(cameraPosition, car.position, up);
        mvp.view = glm::lookAt(cameraPosition, car.position + front, glm::vec3(0.0f, 1.0f, 0.0f));

    }
    glm::mat4 carModel = glm::translate(glm::mat4(1.0f), car.position);
    carModel = glm::rotate(carModel, glm::radians(-car.yaw), car.getUp()); 
    glUseProgram(modelPrograms[cube]);
    int uniformLoc = glGetUniformLocation(modelPrograms[cube], "model");
    glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(mvp.model * carModel));
    uniformLoc = glGetUniformLocation(modelPrograms[cube], "view");
    glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(mvp.view));
    uniformLoc = glGetUniformLocation(modelPrograms[cube], "projection");
    glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(mvp.projection));
    models[cube].drawModel(modelPrograms[cube]);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
    glDepthMask(GL_FALSE);
    glUseProgram(modelPrograms[sphere]);
    uniformLoc = glGetUniformLocation(modelPrograms[sphere], "model");
    glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(mvp.model));
    uniformLoc = glGetUniformLocation(modelPrograms[sphere], "view");
    glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(mvp.view));
    uniformLoc = glGetUniformLocation(modelPrograms[sphere], "projection");
    glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(mvp.projection));
    glUseProgram(modelPrograms[sphere]);
    models[sphere].drawModel(modelPrograms[sphere]);
    glDepthMask(GL_TRUE);

    createDebugSnapshot();
}


