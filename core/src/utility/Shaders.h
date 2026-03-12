#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct ShaderInfo {
    GLenum type;
    const char* filePath;
};

GLuint loadShaders(ShaderInfo* info);
