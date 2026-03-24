#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "SecondaryLayer.h"
#include <iostream>
#include "shaderLoader.h"
#include "FilePaths.h"
#include "renderingUtilities.h"

// Base layer code from Addison Wesley OpenGL Redbook.

SecondaryLayer::SecondaryLayer() {}

void SecondaryLayer::loadData(const std::shared_ptr<Window>& window, FilePaths* filePaths) {
    setUpFramebuffer(&framebuffer, &renderTexture);

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
        {GL_VERTEX_SHADER, "example/passthrough.vert"},
        {GL_FRAGMENT_SHADER, "example/red.frag"},
        {GL_NONE, NULL},
    };

    program = loadShaders(shaders, filePaths);

    glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(vPosition);
}

SecondaryLayer::~SecondaryLayer() {
    std::cout << "Deleting base layer resources." << std::endl;
}

void SecondaryLayer::onUpdate(float timestep) {

}

void SecondaryLayer::onEvent(Event& event) {

}

void SecondaryLayer::onRender(const std::shared_ptr<Window>& window, FilePaths* filePaths) {
    glUseProgram(program);

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glViewport(0, 0, 1920, 1080);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(VAOs[Triangles]);
    glDrawArrays(GL_TRIANGLES, 0, NumVertices); 
}

