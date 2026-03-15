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
