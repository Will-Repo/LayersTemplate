#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct ShaderInfo {
    GLenum type;
    const char* filePath; //File path from shaders directory.
};

GLuint loadShaders(ShaderInfo* info);
GLuint compileShader(GLenum type, const char* filePath);
const char* getFileContents(const char* filePath);
