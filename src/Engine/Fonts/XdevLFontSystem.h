#ifndef XDEVL_FONT_SYSTEM_H
#define XDEVL_FONT_SYSTEM_H

#include "XdevLFont.h"

namespace soan {

    class XdevLFontSystem {
        public:
            /// The function pointer that creates a texture out of an image.
            typedef xdl::XdevLTexture* (*createTextureFromFileCallbackFunction)(const xdl::xdl_char* imageFileName);

            XdevLFontSystem();

            virtual ~XdevLFontSystem();


            /// Create a font from the font file.
            virtual XdevLFont* createFromFontFile(const xdl::xdl_char* fontInfoFilename) = 0;

            /// Create font from texture.
            virtual XdevLFont* createFontFromTexture(const xdl::xdl_char* fontInfoFilename, xdl::XdevLTexture* texture) = 0;

            /// Sets the function which imports a image and creates a texture.
            virtual void setCreateTextureCallback(createTextureFromFileCallbackFunction function) = 0;
    };

}

#endif // XDEVL_FONT_SYSTEM_H
