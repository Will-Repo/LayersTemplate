#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>
#include <string>

class TextRenderer {
    public:
        TextRenderer();
        static void addFace(std::string name, std::string filePath);
        static std::map<char, Character> getCharacters(FT_Face face);
        static void renderText(std::string faceName, std::string text, int posX, int posY, int pixelSize);
    private:
        static FT_Library library;
        struct Character {
            unsigned int TextureID;
            glm::ivec2   Size;
            glm::ivec2   Bearing;
            unsigned int NextOffset;    // Offset to advance to next glyph
        };
        static std::map<std::string, std::pair(FT_Face, std::map<char, Character>)> faces;
};
