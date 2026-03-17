#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "BaseLayer.h"
#include <iostream>
#include "shaderLoader.h"
#include "TextRenderer.h"
#include "Window.h"
#include "FilePaths.h"

// Base layer code from Addison Wesley OpenGL Redbook.

BaseLayer::BaseLayer() {}

void BaseLayer::loadData(Window* window, FilePaths* filePaths) {
    // Load fonts. Function automatically checks if layer has been loaded already.
    window->textRenderer.addFace("bitcount", "example/Bitcount.ttf", filePaths);
    window->textRenderer.addFace("iosevka", "example/Iosevka.ttf", filePaths);

    // Other data - should probably move this elsewhere if the data changes.
    glGenVertexArrays(NumVAOs, VAOs);
    glBindVertexArray(VAOs[Triangles]);

    GLfloat vertices[NumVertices][2] = {
        {-0.9, -0.9},
        {0.85, -0.9},
        {-0.9, 0.85},
        {0.9, -0.85},
        {0.9, 0.9},
        {-0.85, 0.9}
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

BaseLayer::~BaseLayer() {
    std::cout << "Deleting base layer resources." << std::endl;
}

void BaseLayer::onUpdate(float timestep) {

}

void BaseLayer::onEvent(Event& event) {

}

void BaseLayer::onRender(Window* window, FilePaths* filePaths) {
    glUseProgram(program);
    glBindVertexArray(VAOs[Triangles]);
    glDrawArrays(GL_TRIANGLES, 0, NumVertices);
    window->textRenderer.renderText("bitcount", "Application Template", 800, 540, 0.5f, glm::vec3(0, 255, 0), filePaths);
    window->textRenderer.renderText("iosevka", "...", 800, 520, 0.5f, glm::vec3(0, 255, 0), filePaths);
}

