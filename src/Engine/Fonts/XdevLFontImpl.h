#ifndef XDEVL_FONT_IMPL_H
#define XDEVL_FONT_IMPL_H

#include <XdevLWindow/XdevLWindow.h>
#include <XdevLOpenGL/XdevLOpenGL.h>
#include <Engine/Fonts/XdevLFont.h>
#include <map>

// TODO Add alignment stuff, like LEFT_TOP, TOP, RIGHT_TOP etc.

namespace soan {


	class XdevLFontImpl : public XdevLFont {
		public:
			typedef std::vector<xdl::XdevLTexture*> XdevLTextureArray;
			typedef std::map<xdl::xdl_uint32, XdevLGlyphMetric> XdevLGlyphMetricMap;
			
			XdevLFontImpl(xdl::XdevLWindow* window, xdl::XdevLOpenGL330* openGL);

			virtual ~XdevLFontImpl();

			virtual xdl::xdl_int createFontFromTexture(const xdl::xdl_char* fontInfoFilename, xdl::XdevLTexture* texture);
			virtual XdevLGlyphMetric& getGlyphMetric(xdl::xdl_uint32 unicode);
			virtual xdl::xdl_float getFontSize();
			virtual xdl::xdl_float getNewLineSize();
			virtual xdl::xdl_uint getNumberOfTextures();
			virtual xdl::XdevLTexture* getTexture(xdl::xdl_uint idx);
			
			virtual xdl::xdl_int importFromFontFile(const xdl::xdl_char* fontInfoFilename);
			virtual void setCreateTextureCallback(createTextureFromFileCallbackFunction function);
		private:
			/// Reads one line.
			XdevLGlyphMetric& readLine(std::ifstream& os, XdevLGlyphMetric& gp);
			void calculateGlyphInformation(std::ifstream& os);
		private:
			xdl::XdevLWindow*	m_window;
			xdl::XdevLOpenGL330*		m_openGL;
			XdevLTextureArray				m_textureList;
			xdl::xdl_uint						m_numberOfTextures;
			xdl::xdl_float 					m_fontSize;
			xdl::xdl_float					m_newLine;
			xdl::xdl_float 					m_unitX;
			xdl::xdl_float 					m_unitY;
			XdevLGlyphMetricMap			m_glyphMap;
			XdevLGlyphMetric				m_dummyGlyph;
			createTextureFromFileCallbackFunction createTextureFromFile;
	};

}

#endif
