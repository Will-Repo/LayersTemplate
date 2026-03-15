#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shaderLoader.h"
#include <iostream>
#include <string>
#include "fileReading.h"

GLuint loadShaders(ShaderInfo* info) {
    GLuint program = glCreateProgram();
    std::cout << "Entered" << std::endl;
    for (int enumType = 0; info[enumType].type != GL_NONE; enumType++) {
        const char* filePath = info[enumType].filePath;
        GLuint shader = compileShader(info[enumType].type, filePath);
        glAttachShader(program, shader);
        glDeleteShader(shader);
    }
    glLinkProgram(program);
    //TODO: Error check.
    
    return program;
}

GLuint compileShader(GLenum type, const char* filePath) {
    std::string sourceString = getFileContents((std::string("shaders/") + filePath).c_str());
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
