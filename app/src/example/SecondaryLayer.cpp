#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "SecondaryLayer.h"
#include <iostream>
#include "Shaders.h"

// Base layer code from Addison Wesley OpenGL Redbook.

SecondaryLayer::SecondaryLayer() {}

void SecondaryLayer::loadData() {
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

    program = loadShaders(shaders);

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

void SecondaryLayer::onRender() {
    glUseProgram(program);
    glBindVertexArray(VAOs[Triangles]);
    glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

