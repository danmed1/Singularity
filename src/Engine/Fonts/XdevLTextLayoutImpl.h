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

#ifndef XDEVL_TEXT_LAYOUT_IMPL_H
#define XDEVL_TEXT_LAYOUT_IMPL_H

#include <XdevLWindow/XdevLWindow.h>
#include <XdevLOpenGL/XdevLOpenGL.h>
#include <Engine/Fonts/XdevLTextLayout.h>
#include <map>

namespace xdl {

	enum XdevLTextVertexAttributeId {
	    XDEVL_VERTEX_POSITION		= 0,
	    XDEVL_VERTEX_NORMAL			= 1,
	    XDEVL_VERTEX_TANGENT		= 2,
	    XDEVL_VERTEX_BITANGENT		= 3,
	    XDEVL_VERTEX_COLOR			= 4,
	    XDEVL_VERTEX_TEXTURE_COORD	= 9
	};


	class XdevLTextLayoutImpl : public XdevLTextLayout {
		public:

			XdevLTextLayoutImpl(XdevLWindow* window, XdevLOpenGL330* openGL);

			virtual ~XdevLTextLayoutImpl() {}

			virtual xdl_int init() override;

			virtual void useFont(XdevLFont* font) override;

			virtual void printText(const std::wstring& text, xdl_float x, xdl_float y) override;

			virtual void addDynamicText(const std::wstring&  text, xdl_float x, xdl_float y) override;

			virtual void setScale(xdl_float scale) override;

			virtual void setDPI(xdl_float) override;

			virtual void setColor(xdl_uint32 r, xdl_uint32 g, xdl_uint32 b, xdl_uint32 a) override;

			virtual void setDFT(xdl_bool enable) override;

			virtual void setEffect(xdl_uint effectNumber) override;

			virtual void setShadowOffset(xdl_float xOffset, xdl_float yOffset) override;

			virtual void render() override;

		private:

			/// Creates a layout and vertex buffer for the GPU.
			void layoutVertexBuffer(std::vector<XdevLTextLayoutText>& textList, std::map<xdl_uint, std::vector<XdevLGlyphVertex> >& vertexList);

		private:
			XdevLWindow*			m_window;
			XdevLOpenGL330*			m_openGL;
			XdevLFont*				m_font;
			XdevLShaderProgram* 	m_shaderProgram;
			XdevLVertexShader*		m_vertexShader;
			XdevLFragmentShader*	m_fragmentShader;

			std::string				m_vertexShaderFilename;
			std::string				m_fragmentShaderFilename;

			XdevLTexture* 			m_texture;
			XdevLVertexArray*		m_vertexArray;
			XdevLVertexDeclaration* m_vd;
			XdevLVertexBuffer* 		m_vertexBuffer;

			XdevLVertexArray*		m_staticVertexArray;
			XdevLVertexBuffer* 		m_staticVertexBuffer;

			xdl_int					m_projViewModelMatrix;
			xdl_int					m_texture0;
			xdl_int					m_color;

			xdl_int					m_gammaid;
			xdl_int					m_bufferid;
			xdl_int					m_dftid;
			xdl_float				m_gamma;
			xdl_float				m_buffer;
			xdl_int					m_dft;
			xdl_int					m_effectid;
			xdl_int					m_shadowOffsetid;

			xdl_float				m_dpi;
			xdl_float				m_scale;
			xdl_float				m_rescale;
			xdl_uint8				m_currentColor[4];
			xdl_float				m_shadowOffset[2];
			xdl_uint				m_effectNumber;

			std::vector<XdevLTextLayoutText> m_textList;
			std::vector<XdevLTextLayoutText> m_staticTextList;
			xdl_bool					m_staticVertexListUploaded;

			std::map<xdl_uint, std::vector<XdevLGlyphVertex>> 	m_simpleTextVertexMap;
			std::map<xdl_uint, std::vector<XdevLGlyphVertex>> 	m_dynamicVertexMap;
	};

}

#endif // XDEVL_TEXT_LAYOUT_H
