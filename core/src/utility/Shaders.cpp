#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shaders.h"
#include <iostream>
#include <string>
#include <fstream>

GLuint loadShaders(ShaderInfo* info) {
    GLuint program;
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
    const char* source = getFileContents(filePath);
    int success;
    char infoLog[512];
    
    GLuint shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "Shader compilation failed: " << type << ", " << infoLog << std::endl;
    };

    return shader;
}

//File path is from shaders directory.
const char* getFileContents(const char* filePath) {
    std::string contents;
    std::ifstream inputStream(std::string("../app/shaders/") + filePath);

    std::string line;
    while (getline (inputStream, line)) {
        contents += line + "\n";
    }

    std::cout << contents << std::endl;

    inputStream.close();
    
    return contents.c_str();
}
