#ifndef XDEVL_FONT_SYSTEM_IMPL_H
#define XDEVL_FONT_SYSTEM_IMPL_H

#include "XdevLFontSystem.h"
#include <XdevLOpenGL/XdevLOpenGL.h>


namespace soan {

    class XdevLFontImpl;
    
    class XdevLFontSystemImpl {
        public:
            XdevLFontSystemImpl();
            virtual ~XdevLFontSystemImpl();

            virtual XdevLFont* createFromFontFile(const xdl::xdl_char* fontInfoFilename);

            /// Create font from texture.
            virtual XdevLFont* createFontFromTexture(const xdl::xdl_char* fontInfoFilename, xdl::XdevLTexture* texture);

            /// Sets the function which imports a image and creates a texture.
            virtual void setCreateTextureCallback(XdevLFontSystem::createTextureFromFileCallbackFunction function);
            
        private:
            XdevLGlyphMetric& readLine(std::ifstream& os, XdevLGlyphMetric& gp);
            void calculateGlyphInformation(XdevLFontImpl* font, xdl::xdl_int textureWidth, std::ifstream& os);
        private:
            xdl::xdl_uint screenWidth;
            xdl::xdl_uint screenHeight;
            xdl::XdevLOpenGL330* openGL;
            XdevLFontSystem::createTextureFromFileCallbackFunction createTextureFromFile;
    };

}

#endif // XDEVL_FONT_SYSTEM_H
