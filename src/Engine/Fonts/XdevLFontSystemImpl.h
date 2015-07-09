#ifndef XDEVL_FONT_SYSTEM_IMPL_H
#define XDEVL_FONT_SYSTEM_IMPL_H

#include "XdevLFontSystem.h"
#include <XdevLOpenGL/XdevLOpenGL.h>


namespace soan {

	class XdevLFontImpl;

	class XdevLFontSystemImpl : public XdevLFontSystem {
		public:

			XdevLFontSystemImpl();

			virtual ~XdevLFontSystemImpl();

			virtual xdl::xdl_int init(xdl::xdl_uint screenWidth, xdl::xdl_uint screenHeight, xdl::XdevLOpenGL330* opengl) override;

			virtual XdevLFont* createFromFontFile(const xdl::xdl_char* fontInfoFilename) override;

			/// Create font from texture.
			virtual XdevLFont* createFontFromTexture(const xdl::xdl_char* fontInfoFilename, xdl::XdevLTexture* texture) override;

			/// Sets the function which imports a image and creates a texture.
			virtual void setCreateTextureCallback(XdevLFontSystem::createTextureFromFileCallbackFunction function) override;

		private:
			XdevLGlyphMetric& readLine(std::ifstream& os, XdevLGlyphMetric& gp);
			void calculateGlyphInformation(XdevLFontImpl* font, std::ifstream& os);
		private:
			xdl::xdl_uint screenWidth;
			xdl::xdl_uint screenHeight;
			xdl::XdevLOpenGL330* openGL;
			XdevLFontSystem::createTextureFromFileCallbackFunction createTextureFromFile;
	};

}

#endif // XDEVL_FONT_SYSTEM_H
