#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "TextRenderer.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>
#include <map>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shaderLoader.h"
#include <glm/gtc/type_ptr.hpp>
#include "FilePaths.h"

TextRenderer::TextRenderer() {
    int error = FT_Init_FreeType(&library);
    if (error) {
        //TODO: Error handling
        std::cout << "Error initialising freetype" << std::endl;
    }
}

//File path is from fonts directory.
void TextRenderer::addFace(std::string name, std::string filePath, FilePaths* filePaths) {
    if (faces.find(name) != faces.end()) {
        std::cout << "Font already loaded" << std::endl;
        return;
    }
    FT_Face face;
    filePath = filePaths->executablePath + "/" +  filePaths->fontsPath + "/" + filePath;
    int error = FT_New_Face(library, filePath.c_str(), 0, &face); //TODO: switch this to config struct filepath.
    //TODO: error handiling
    if (error == FT_Err_Unknown_File_Format) {
        std::cout << "The font file is readable, but has an invalid/unsupported format." << std::endl;
    } else if (error) {
        std::cout << "Loading new face resulted in error: " << error << "." << std::endl;
    }

    FT_Set_Pixel_Sizes(face, 0, 48);

    //Load the font's characters for easy retrieval later.
    std::map<char, Character> chars = getCharacters(face);
    faces.insert(std::pair(name, std::pair(face, chars)));
}

// Restricted to 1st 128 of ascii for now.
std::map<char, TextRenderer::Character> TextRenderer::getCharacters(FT_Face face) {
    // Code from learn opengl
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
    std::map<char, Character> chars;
    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }

        unsigned int texture = 0;
        if (face->glyph->bitmap.width > 0 && face->glyph->bitmap.rows > 0) {
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        } else {
            std::cout << "Empty glyph for character: " << c << "." << std::endl;
        }
        Character character = {
            texture, 
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),                
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        chars.insert(std::pair<char, Character>(c, character));
    }
    return chars;
}

//TODO: Perhaps alert app if invalid face, or just log error and change to default face.
void TextRenderer::renderText(std::string faceName, std::string text, float x, float y, float scale, glm::vec3 colour, FilePaths* filePaths) {
    // Code here from https://learnopengl.com/In-Practice/Text-Rendering, will write my own once verified working.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
    glm::mat4 projection = glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f);
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); 

    ShaderInfo shaders[] = {
        {GL_VERTEX_SHADER, "text.vert"},
        {GL_FRAGMENT_SHADER, "text.frag"},
        {GL_NONE, NULL},
    };

    GLuint program = loadShadersCore(shaders, filePaths);
    glUseProgram(program);
    glUniform3f(glGetUniformLocation(program, "textColor"), colour.x, colour.y, colour.z);
    glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    auto it = faces.find(faceName);
    std::map<char, Character> Characters = it->second.second;
    
    //TODO: Name error check
    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },            
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }           
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.NextOffset >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
}
