#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>
#include <string>

class TextRenderer {
    public:
        TextRenderer();
        void addFace(std::string name, std::string filePath);
        void renderText(FT_Face face, std::string text, int posX, int posY, int pixelSize);
    private:
        FT_Library library;
        std::map<std::string, FT_Face> faces;
};
