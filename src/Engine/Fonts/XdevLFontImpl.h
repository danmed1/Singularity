/*
	Copyright (c) 2015 Cengiz Terzibas

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE.

	cengiz@terzibas.de
*/

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

			XdevLFontImpl();

			virtual ~XdevLFontImpl();

			virtual XdevLGlyphMetric& getGlyphMetric(xdl::xdl_uint32 unicode);
			virtual xdl::xdl_float getFontSize();
			virtual xdl::xdl_float getNewLineSize();
			virtual xdl::xdl_uint getNumberOfTextures();
			virtual xdl::XdevLTexture* getTexture(xdl::xdl_uint idx);

		public:

			void addFontTexture(xdl::XdevLTexture* fontTexture) {
				m_textureList.push_back(fontTexture);
			}

			xdl::XdevLTexture* getFontTexture(const XdevLGlyphMetric& gp) {
				return m_textureList[gp.tid];
			}

			void setNewLineSize(xdl::xdl_float newLineSize) {
				m_newLine = newLineSize;
			}

			void setUnitX(xdl::xdl_float unitX) {
				m_unitX = unitX;
			}

			void setUnitY(xdl::xdl_float unitY) {
				m_unitY = unitY;
			}

			void setFontSize(xdl::xdl_uint fontSize) {
				m_fontSize = fontSize;
			}

			void addGlyph(const XdevLGlyphMetric& glyph) {
				m_glyphMap[glyph.character_code] = glyph;
			}

		private:

			xdl::xdl_float 			m_fontSize;
			xdl::xdl_float			m_newLine;
			xdl::xdl_float 			m_unitX;
			xdl::xdl_float 			m_unitY;
			XdevLGlyphMetric		m_dummyGlyph;
			XdevLGlyphMetricMap		m_glyphMap;
			XdevLTextureArray		m_textureList;
	};

}

#endif
