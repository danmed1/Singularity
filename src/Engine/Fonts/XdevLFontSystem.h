#ifndef XDEVL_FONT_SYSTEM_H
#define XDEVL_FONT_SYSTEM_H

#include <XdevLOpenGL/XdevLOpenGL.h>
#include "XdevLFont.h"

namespace xdl {

	
	class XdevLFontSystem {
		public:
			/// The function pointer that creates a texture out of an image.
			typedef XdevLTexture* (*createTextureFromFileCallbackFunction)(const xdl_char* imageFileName);

			virtual ~XdevLFontSystem() {};

			virtual xdl_int init(xdl_uint screenWidth, xdl_uint screenHeight, XdevLOpenGL330* opengl) = 0;

			/// Create a font from the font file.
			virtual XdevLFont* createFromFontFile(const xdl_char* fontInfoFilename) = 0;

			/// Create font from texture.
			virtual XdevLFont* createFontFromTexture(const xdl_char* fontInfoFilename, XdevLTexture* texture) = 0;

			/// Sets the function which imports a image and creates a texture.
			virtual void setCreateTextureCallback(createTextureFromFileCallbackFunction function) = 0;
	};

}

#endif // XDEVL_FONT_SYSTEM_H
