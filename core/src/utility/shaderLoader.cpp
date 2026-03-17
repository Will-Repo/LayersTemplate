#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shaderLoader.h"
#include <iostream>
#include <string>
#include "fileReading.h"
#include "FilePaths.h"

GLuint loadShaders(ShaderInfo* info, FilePaths* filePaths) {
    GLuint program = glCreateProgram();
    for (int enumType = 0; info[enumType].type != GL_NONE; enumType++) {
        const char* filePath = info[enumType].filePath;
        std::string sourceString = getFileContents((filePaths->executablePath + "/" + filePaths->shadersPath + "/" + filePath).c_str());
        GLuint shader = compileShader(info[enumType].type, sourceString);
        glAttachShader(program, shader);
        glDeleteShader(shader);
    }
    glLinkProgram(program);
    //TODO: Error check.
    
    return program;
}

GLuint compileShader(GLenum type, std::string sourceString) {
    const char* source = sourceString.c_str();
    int success;
    char infoLog[512];
    
    //std::cout << source << std::endl;
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "Shader compilation failed: " << type << ", " << infoLog << std::endl;
    };

    return shader;
}

// If calling from core, use fixed core path instead.
GLuint loadShaders(ShaderInfo* info, FilePaths* filePaths, std::string check) {
    if (check != "core") {
        std::cout << "This function uses the core file path, use loadShaders without the second parameter to use the app/ user defined path" << std::endl;
        exit(1);
    }
    GLuint program = glCreateProgram();
    for (int enumType = 0; info[enumType].type != GL_NONE; enumType++) {
        const char* filePath = info[enumType].filePath;
        std::string sourceString = getFileContents((filePaths->executablePath + "/" + filePaths->corePath + "/shaders/" + filePath).c_str());
        GLuint shader = compileShader(info[enumType].type, sourceString);
        glAttachShader(program, shader);
        glDeleteShader(shader);
    }
    glLinkProgram(program);
    //TODO: Error check.
    
    return program;
}

