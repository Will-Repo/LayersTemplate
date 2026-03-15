#include "TextRenderer.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>

TextRenderer::TextRenderer() {
    int error = FT_Init_FreeType(&library);
    if (error) {
        //TODO: Error handling
        std::cout << "Error initialising freetype" << std::endl;
    }
}

//File path is from fonts directory.
void TextRenderer::addFace(std::string name, std::string filePath) {
    FT_Face face;
    filePath = "../../app/fonts/" + filePath;
    int error = FT_New_Face(library, filePath.c_str(), 0, &face); //TODO: switch this to config struct filepath.
    //TODO: error handling
    if (error == FT_Err_Unknown_File_Format) {
        std::cout << "The font file is readable, but has an invalid/unsupported format." << std::endl;
    } else if (error) {
        std::cout << "Loading new face resulted in error." << std::endl;
    }

    faces.insert(std::pair{name, face});
}

//TODO: Perhaps alert app if invalid face, or just log error and change to default face.
void TextRenderer::renderText(FT_Face face, std::string text, int posX, int posY, int pixelSize) {
    
}
