#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>
#include <string>
#include <glm/glm.hpp>
#include "FilePaths.h"

class TextRenderer {
    public:
        TextRenderer();
        struct Character {
            unsigned int TextureID;
            glm::ivec2   Size;
            glm::ivec2   Bearing;
            unsigned int NextOffset;    // Offset to advance to next glyph
        };
        void addFace(std::string name, std::string filePath, FilePaths* filePaths);
        std::map<char, Character> getCharacters(FT_Face face);
        void renderText(std::string faceName, std::string text, float x, float y, float scale, glm::vec3 colour, FilePaths* filePaths);
    private:
        FT_Library library;
        std::map<std::string, std::pair<FT_Face, std::map<char, Character>>> faces;
};
