#include "RenderingThread.h"
#include <thread>
#include <iostream>
#include "Window.h"
#include "FilePaths.h"
#include "shaderLoader.h"

RenderingThread::RenderingThread(FilePaths* paths) : filePaths(paths) {
}

RenderingThread::~RenderingThread() {
    if (thread.joinable()) {
        thread.join(); // Wait for thread to finish before destroying class object to prevent core dump.
    }
}

void RenderingThread::startRendering() {
    started = true;
    thread = std::thread(&RenderingThread::renderWindows, this);
}

// Function for setting up shader program and VAO used for combining layer's textures. Requires to be called when context used is removed - e.g. when window is closed.
static void setupTextureRender(Window::Quad& data, FilePaths* paths) {
// Make program object for rendering quad with textures to screen.
    ShaderInfo shaders[] = {
        {GL_VERTEX_SHADER, "quad.vert"},
        {GL_FRAGMENT_SHADER, "quad.frag"},
        {GL_NONE, NULL},
    };

    data.program = loadShadersCore(shaders, paths);

    glGenVertexArrays(1, &data.VAO);
    glBindVertexArray(data.VAO);

    float textureCoords[] = {
        // OpenGL coords, texture coords.
        -1.0f, -1.0f, 0.0f, 0.0f, // Bottom left
        -1.0f, 1.0f, 0.0f, 1.0f,  // Top left
        1.0f, -1.0f, 1.0f, 0.0f,  // Bottom right
        1.0f, 1.0f, 1.0f, 1.0f,    // Top right
    };

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoords), textureCoords, GL_STATIC_DRAW);

    unsigned int indices[] = {
        0, 1, 2, // Bottom left triangle
        1, 2, 3 // Top right triangle
    };

    glGenBuffers(1, &data.EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Vertices
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coords
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

// Loop at max window render limit, but limit each window to its own, and each layer to its own limit.
void RenderingThread::renderWindows() {
    int maxFrameLimit = 0;
    for (const auto& windowPtr : windows) {
        // Make shared pointer from weak pointer, if window has not closed.
        if (auto window = windowPtr.lock()) {
            if (window->config.renderingFrameLimit > maxFrameLimit) {
                maxFrameLimit = window->config.renderingFrameLimit;
            }
        }
    }
    float frameTime = 1.0 / maxFrameLimit;

    float deltaTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> oldDelta;

    //Timestep variables.
    float timestep;
    std::chrono::time_point<std::chrono::high_resolution_clock> now;

    do {
        while (!newWindowQueueIsEmpty()) {
            auto windowPtr = dequeueNewWindow();
            if (auto window = windowPtr.lock()) {
                glfwMakeContextCurrent(window->getWindow());
                setupTextureRender(window->quad, filePaths);
                windows.push_back(window);
            }
        }

        for (auto it = windows.begin(); it != windows.end();) {
            // Check if window has closed.
            if (auto window = it->lock()) {
                float windowFrameTime = 1.0 / window->config.renderingFrameLimit;
                now = std::chrono::high_resolution_clock::now();
                if (std::chrono::duration<float>(now - window->lastRendered).count() >= windowFrameTime) {            
                    //std::cout << "Making window " << window->config.windowName << " current." << std::endl;
                    //std::cout << "Window: " << window->getWindow() << "." << std::endl;
                    glfwMakeContextCurrent(window->getWindow());

                    glBindFramebuffer(GL_FRAMEBUFFER, 0);
                    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                    glClear(GL_COLOR_BUFFER_BIT);
    
                    // Render each layer, if its passed their frame time.
                    for (auto& layer : window->layerStack) {
                        if (!layer->renderSetupComplete) {
                            std::cout << "Loading render data" << std::endl;
                            layer->loadRenderData(window.get(), filePaths);
                        }
                        // See if enough time has elapsed to call for update.
                        now = std::chrono::high_resolution_clock::now();
                        timestep = std::chrono::duration<float>(now - layer->lastRendered).count();
                        if (timestep >= (1.0 / layer->config.renderingFrameLimit)) {
                            layer->lastRendered = now;
                            // Update the stored texture of the layer.
                            layer->onRender(filePaths); 
                        }
                    }
    
                    // Prevent layers overwriting other layers. TODO: Move out of loop if possible.
                    glEnable(GL_BLEND);
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

                    // Render all textures to the screen.
                    glUseProgram(window->quad.program);
                    glDisable(GL_DEPTH_TEST);
                    glBindFramebuffer(GL_FRAMEBUFFER, 0);
                    glViewport(0, 0, 1920, 1080); //TODO: Make window dimensions.
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, window->quad.EBO);
                    for (auto& layer : window->layerStack) {
                        glActiveTexture(GL_TEXTURE0);
                        glBindTexture(GL_TEXTURE_2D, layer->renderTexture);
                        glUniform1i(glGetUniformLocation(window->quad.program, "ourTexture"), 0);  
                        glBindVertexArray(window->quad.VAO);
                        // Draw quad, shaders will ensure texture is drawn to screen.
                        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                    }
                    glfwSwapBuffers(window->getWindow());
                } 
                ++it;
            } else {
                it = windows.erase(it);
            }
        }

        // Calculating deltatime, in seconds.
        deltaTime = std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - oldDelta).count();
        if (frameTime - deltaTime > 0) {
            std::this_thread::sleep_for(std::chrono::duration<float> (frameTime - deltaTime));
        }
        oldDelta = std::chrono::high_resolution_clock::now();
    } while (windows.size() > 0);
}

void RenderingThread::addWindow(std::weak_ptr<Window> windowPtr) {
    enqueueNewWindow(std::move(windowPtr));
}
