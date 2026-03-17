#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include "FilePaths.h"

struct ShaderInfo {
    GLenum type;
    const char* filePath; //File path from shaders directory.
};

GLuint loadShaders(ShaderInfo* info, FilePaths* filePaths);
GLuint compileShader(GLenum type, std::string sourceString);

GLuint loadShaders(ShaderInfo* info, FilePaths* filePaths, std::string check);
