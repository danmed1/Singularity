#ifndef XDEVL_FONT_SYSTEM_IMPL_H
#define XDEVL_FONT_SYSTEM_IMPL_H

#include "XdevLFontSystem.h"
#include <XdevLOpenGL/XdevLOpenGL.h>


namespace xdl {

	class XdevLFontImpl;

	class XdevLFontSystemImpl : public XdevLFontSystem {
		public:

			XdevLFontSystemImpl();

			virtual ~XdevLFontSystemImpl();

			virtual xdl_int init(xdl_uint screenWidth, xdl_uint screenHeight, XdevLOpenGL330* opengl) override;

			virtual XdevLFont* createFromFontFile(const xdl_char* fontInfoFilename) override;

			/// Create font from texture.
			virtual XdevLFont* createFontFromTexture(const xdl_char* fontInfoFilename, XdevLTexture* texture) override;

			/// Sets the function which imports a image and creates a texture.
			virtual void setCreateTextureCallback(XdevLFontSystem::createTextureFromFileCallbackFunction function) override;

		private:
			XdevLGlyphMetric& readLine(std::ifstream& os, XdevLGlyphMetric& gp);
			void calculateGlyphInformation(XdevLFontImpl* font, std::ifstream& os);
		private:
			xdl_uint screenWidth;
			xdl_uint screenHeight;
			XdevLOpenGL330* openGL;
			XdevLFontSystem::createTextureFromFileCallbackFunction createTextureFromFile;
	};

}

#endif // XDEVL_FONT_SYSTEM_H
