#include <ft2build.h>
#include FT_FREETYPE_H

class TextRenderer {
    public:
        TextRenderer();
    private:
        FT_Library library;
};
