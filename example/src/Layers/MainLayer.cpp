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
#include "Physics.h"

// Base layer code from Addison Wesley OpenGL Redbook.

MainLayer::MainLayer() {}
            
DebugInfo MainLayer::getDebugInfo() {
    std::lock_guard<std::mutex> lock(debugMutex); 
    return debugInfo;
}

void MainLayer::createDebugSnapshot() {
    btTransform transform;
    {
        std::lock_guard<std::mutex> lock(physicsMutex); 
        carBody->getMotionState()->getWorldTransform(transform);
    }
    std::lock_guard<std::mutex> lock(debugMutex); 
    //FPS
    debugInfo.fps = 1.0f / timestep;
    //POSITION
    btVector3 pos = transform.getOrigin();
    debugInfo.position = glm::vec3(pos.x(), pos.y(), pos.z());
    //ROTATION
    btQuaternion carRotation = transform.getRotation();
    btScalar yaw, pitch, roll;
    carRotation.getEulerZYX(yaw, pitch, roll);
    debugInfo.rotation = glm::vec3(yaw, pitch, roll);
}

void MainLayer::physicsSetup() {
    std::lock_guard<std::mutex> lock(physicsMutex); 
    // Set up world
    dynamicsWorld->setGravity(btVector3(0, car.gravity, 0));

    // Set up map
    btCollisionShape* worldPhysics = physics.addStaticMap(filepaths->executablePath + "/" + filepaths->assetsPath + "/world/worldMesh.obj");
    collisionShapes.push_back(worldPhysics);        
    btTransform groundTransform;
    groundTransform.setIdentity();
    //groundTransform.setOrigin(btVector3(0, -56, 0));
    groundTransform.setOrigin(btVector3(0, 0, 0));
    btScalar mass(0.);
    //using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
    btDefaultMotionState* worldMotionState = new btDefaultMotionState(groundTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, worldMotionState, worldPhysics);
    worldBody = new btRigidBody(rbInfo);
    //add the body to the dynamics world
    dynamicsWorld->addRigidBody(worldBody);

    // Set up car
    btCollisionShape* carShape = new btBoxShape(btVector3(car.width * 0.5f, car.height * 0.5f, car.length * 0.5f));
    btTransform carTransform;
    carTransform.setIdentity();
    carTransform.setOrigin(btVector3(car.startingPosition.x, car.startingPosition.y, car.startingPosition.z));
    btVector3 inertia(0, 0, 0);
    carShape->calculateLocalInertia(car.mass, inertia);
    std::cout << inertia.x() << inertia.y() << inertia.z() <<std::endl;
    btDefaultMotionState* carMotionState = new btDefaultMotionState(carTransform);
    btRigidBody::btRigidBodyConstructionInfo info(car.mass, carMotionState, carShape, inertia);
    carBody = new btRigidBody(info);
    dynamicsWorld->addRigidBody(carBody);
}

void MainLayer::renderingSetup() {
    // Render world - sphere boundary.
    ShaderInfo mvpShaders[] = {
        {GL_VERTEX_SHADER, "mvp.vert", ShaderDataType::Path},
        {GL_FRAGMENT_SHADER, "mvp.frag", ShaderDataType::Path},
        {GL_NONE, NULL, ShaderDataType::Path},
    };    
    std::string path = filepaths->executablePath + "/" + filepaths->shadersPath;
    modelPrograms[world] = loadShaders(mvpShaders, path);    
    glUseProgram(modelPrograms[world]);
    Model worldModel = Model(filepaths->executablePath + "/" + filepaths->assetsPath + "/world/world.obj");
    models[world] = worldModel;

    modelPrograms[player] = loadShaders(mvpShaders, path);    
    glUseProgram(modelPrograms[player]);
    Model carModel = Model(filepaths->executablePath + "/" + filepaths->assetsPath + "/cars/default/car.obj");
    models[player] = carModel;
}

void MainLayer::loadData(Window* window, FilePaths* filepaths) {
    // Set up fbo to be rendered to - prevents mismatching fps causing layers to flicker (i.e. not be displayed on some frames).
    setupLayer(window, filepaths);

    physicsSetup();
    renderingSetup();
    
    // Set MVP matrix initial values.
    camera.mvp.projection = glm::perspective(glm::radians(45.0f), (float)1920 / (float)1080, 0.1f, 100.0f);
    mvp.projection = glm::perspective(glm::radians(45.0f), (float)1920 / (float)1080, 0.1f, 100.0f);

    glfwSetInputMode(window->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    renderSetupComplete = true;
}

MainLayer::~MainLayer() {
    std::cout << "Deleting base layer resources." << std::endl;
}

void MainLayer::onUpdate(float timestep) {
    if (!renderSetupComplete) {
        return;
    }
    // Check for collisions.
    {
        std::lock_guard<std::mutex> lock(physicsMutex); 
        dynamicsWorld->stepSimulation(timestep, 10);
        
        // Car
        btTransform transform;
        carBody->getMotionState()->getWorldTransform(transform);
        btVector3 forward = transform.getBasis() * btVector3(1, 0, 0);
        if (carForwardsHeld) {carBody->activate(true);
            //TODO: Need some check that all wheels are on the ground.
            btVector3 velocity = carBody->getLinearVelocity();
            if (velocity.length() < car.maxSpeed) {
                carBody->applyCentralForce(forward * car.engineForce);
            }
            // Clamp velocity to max
            velocity = carBody->getLinearVelocity();
            if (velocity.length() > car.maxSpeed) {
                velocity = velocity.normalized() * car.maxSpeed;
                carBody->setLinearVelocity(velocity);
            }
        }
        /*if (carLeftHeld) {
            car.yaw -= car.turnSpeed * timestep;
            //TODO: Add max car yaw for turning.
        }*/
        if (carBackwardsHeld) {
            btVector3 velocity = carBody->getLinearVelocity();
            if (velocity.length() < car.maxSpeed) {
                carBody->applyCentralForce(-forward * car.engineForce);
            }
            // Clamp velocity to max
            velocity = carBody->getLinearVelocity();
            if (velocity.length() > car.maxSpeed) {
                velocity = velocity.normalized() * car.maxSpeed;
                carBody->setLinearVelocity(velocity);
            }
        }
        /*if (carRightHeld) {
            car.yaw += car.turnSpeed * timestep;
        }*/
    }

    // Apply friction - temp system.float friction = 5.0f;
    /*float friction = 1.0f;
    car.velocity *= expf(-friction * timestep);

    car.position += car.velocity * timestep;*/
    
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

        // If camera attached, change car camera values. Else, modify camera.
        if (cameraAttached) {
            carCamera.yaw   += xOffset;
            carCamera.pitch -= yOffset; 

            carCamera.pitch = std::min(carCamera.pitch, 89.9f);
            carCamera.pitch = std::max(carCamera.pitch, -89.9f);

        } else {
            camera.yaw   += xOffset;
            camera.pitch -= yOffset; 

            camera.pitch = std::min(camera.pitch, 89.9f);
            camera.pitch = std::max(camera.pitch, -89.9f);
        }

        mousePositionEvent->handled = true; 
    }
}

void MainLayer::renderScene(MVP matrix) {
    // Get model matrix for the entire world to be rendered.
    btTransform carTransform;
    btTransform worldTransform;
    {
        std::lock_guard<std::mutex> lock(physicsMutex); 
        carBody->getMotionState()->getWorldTransform(carTransform);
        worldBody->getMotionState()->getWorldTransform(worldTransform);
    }
    btScalar carModel[16];
    carTransform.getOpenGLMatrix(carModel);
    btScalar worldModel[16];
    worldTransform.getOpenGLMatrix(worldModel);


    // Render opaque objects.
    // Render car.
    glm::mat4 carModelMatrix = glm::make_mat4(carModel);
    carModelMatrix = glm::translate(carModelMatrix, glm::vec3(0, car.height * 0.5f, 0));
    glUseProgram(modelPrograms[player]);
    int uniformLoc = glGetUniformLocation(modelPrograms[player], "model");
    glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(carModelMatrix));
    uniformLoc = glGetUniformLocation(modelPrograms[player], "view");
    glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(matrix.view));
    uniformLoc = glGetUniformLocation(modelPrograms[player], "projection");
    glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(matrix.projection));
    models[player].drawModel(modelPrograms[player]);

    // Render transparent objects.
    // Render map.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
    glDepthMask(GL_FALSE);
    glUseProgram(modelPrograms[world]);
    uniformLoc = glGetUniformLocation(modelPrograms[world], "model");
    glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, worldModel);
    uniformLoc = glGetUniformLocation(modelPrograms[world], "view");
    glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(matrix.view));
    uniformLoc = glGetUniformLocation(modelPrograms[world], "projection");
    glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(matrix.projection));
    glUseProgram(modelPrograms[world]);
    models[world].drawModel(modelPrograms[world]);
    glDepthMask(GL_TRUE);
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
        camera.mvp.view = glm::lookAt(camera.position, camera.position + camera.front, camera.up);
        renderScene(camera.mvp);
    } else {
        // Calculate rotation matrix to rotate camera about the car.
        glm::mat4 rotation = glm::yawPitchRoll(glm::radians(carCamera.yaw), glm::radians(carCamera.pitch), 0.0f);
        // Create and rotate base offset - direction from camera to car.
        glm::vec3 offset = glm::vec3(rotation * glm::vec4(0, 0, 1, 0));
        // Get car yaw.
        btTransform transform;

        {
            std::lock_guard<std::mutex> lock(physicsMutex); 
            carBody->getMotionState()->getWorldTransform(transform);
        }
        btQuaternion carRotation = transform.getRotation();
        btScalar yaw, pitch, roll;
        carRotation.getEulerZYX(yaw, pitch, roll);
        // Rotate the camera relative to the car.
        glm::mat4 finalRotation = glm::rotate(glm::mat4(1.0f), glm::radians(180 - yaw), glm::vec3(0, 1, 0));
        glm::vec3 finalOffset = glm::vec3(finalRotation * glm::vec4(offset, 0.0f));  
        // Get car position
        btVector3 pos = transform.getOrigin();
        glm::vec3 glmPos(pos.x(), pos.y(), pos.z());
        // Update view matrix to point from behind and above car, to car (accounting for mouse camera movement).
        glm::vec3 cameraPosition = glmPos - finalOffset * car.length * 5.0f + glm::vec3(0.0f, car.height * 2.5f, 0.0f);
        // Get unit vector to front of car.
        btVector3 forward = transform.getBasis() * btVector3(0, 0, -1);
        glm::vec3 front = glm::normalize(glm::vec3(forward.x(),forward.y(),forward.z()));
        // Look from new camera position to car.
        mvp.view = glm::lookAt(cameraPosition, glmPos + front, glm::vec3(0.0f, 1.0f, 0.0f));
        renderScene(mvp);
    } 

    createDebugSnapshot();
}
