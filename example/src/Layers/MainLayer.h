#pragma once 
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Layer.h"
#include "FilePaths.h"
#include "Model.h"
#include "Car.h"
#include <mutex>
#include "Physics.h"

struct MVP {
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
};

struct Camera {
    glm::vec3 initialPosition = glm::vec3(0.0f, 0.4f, 2.0f);
    glm::vec3 position = initialPosition;

    glm::vec3 front;
    glm::vec3 right;
    glm::vec3 up;

    float speed = 3.0f;

    float yaw = -90.0f;
    float pitch = 0.0f;
    //float roll;

    MVP mvp;
};

struct Mouse {
    double x = 0;
    double y = 0;
    float sensitivity = 0.1f;
};

struct DebugInfo {
    float fps;
};

class MainLayer : public Layer {
    public:
        MainLayer();
        ~MainLayer();
        DebugInfo getDebugInfo();
        void createDebugSnapshot();
        void loadData(Window* window, FilePaths* filePaths) override;
        void onUpdate(float timestep) override;
        void onEvent(std::shared_ptr<Event> event) override;
        void onRender() override;
    private:
        void physicsSetup();
        void renderingSetup();
        void renderScene(MVP mvp);

        enum VAO_IDs {numVAOs}; //Position in enum is number of VAOs.
        GLuint VAOs[numVAOs];
        GLuint numVertices[numVAOs];
        enum Attribute_IDs {vPosition, vColour}; //Default/common values, can use others if necessary.
        GLuint programs[numVAOs];

        enum Model_IDS {world, player, numModels};
        Model models[numModels];
        unsigned int modelPrograms[numModels];

        Camera camera;
        bool cameraAttached = true; // If camera is attached to car movement.
        // Input flags
        bool cameraForwardsHeld = false, cameraLeftHeld = false, cameraBackwardsHeld = false, cameraRightHeld = false, cameraDownHeld = false, cameraUpHeld = false;
        bool cameraSpeedIncreaseHeld = false, cameraSpeedDecreaseHeld = false;

        Mouse mouse;
        bool firstMouse = true;

        // Mvp for car.
        MVP mvp;

        Car car;
        Camera carCamera; //Mouse effected camera for when camera attached to car.
        btRigidBody* carBody;
        // Input flags.
        bool carForwardsHeld = false, carLeftHeld = false, carBackwardsHeld = false, carRightHeld = false;

        // Debug data.
        std::mutex debugMutex;
        DebugInfo debugInfo;
        std::chrono::time_point<std::chrono::high_resolution_clock> lastUpdated;
        float timestep;

        Physics physics;
        std::mutex physicsMutex;
        //TODO: Clean up all of this once i understand it more.
        btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
        btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
        btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();
        btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
        btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
        btAlignedObjectArray<btCollisionShape*> collisionShapes;
};  
