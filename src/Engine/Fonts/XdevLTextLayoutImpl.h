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

namespace soan {



	class XdevLTextLayoutImpl : public XdevLTextLayout {
		public:

			XdevLTextLayoutImpl(xdl::XdevLWindow* window, xdl::XdevLOpenGL330* openGL);

			virtual ~XdevLTextLayoutImpl() {}

			/// Initialize the layout engine.
			virtual xdl::xdl_int init(XdevLFont* font);

			/// Add dynamic text.
			virtual void addDynamicText(const std::wstring&  text, xdl::xdl_float x, xdl::xdl_float y);

			/// Sets the scale factor.
			virtual void setScale(xdl::xdl_float scale);

			/// Sets the DPI.
			virtual void setDPI(xdl::xdl_float);

			/// Sets the current color.
			virtual void setColor(xdl::xdl_float r, xdl::xdl_float g, xdl::xdl_float b);

			/// Sets the font rendering effect.
			virtual void setEffect(xdl::xdl_uint effectNumber);

			/// Sets the shadow offset parameter.
			virtual void setShadowOffset(xdl::xdl_float xOffset, xdl::xdl_float yOffset);

			/// Draw everything.
			virtual void render();

		private:

			/// Creates a layout and vertex buffer for the GPU.
			void layoutVertexBuffer(std::vector<XdevLTextLayoutText>& textList, std::map<xdl::xdl_uint, std::vector<XdevLGlyphVertex> >& vertexList);

		private:
			xdl::XdevLWindow*				m_window;
			xdl::XdevLOpenGL330*					m_openGL;
			XdevLFont*										m_font;
			xdl::XdevLShaderProgram* 			m_shaderProgram;
			xdl::XdevLVertexShader*				m_vertexShader;
			xdl::XdevLFragmentShader*			m_fragmentShader;

			std::string										m_vertexShaderFilename;
			std::string										m_fragmentShaderFilename;

			xdl::XdevLTexture* 						m_texture;
			xdl::XdevLVertexArray*				m_vertexArray;
			xdl::XdevLVertexDeclaration* 	m_vd;
			xdl::XdevLVertexBuffer* 			m_vertexBuffer;

			xdl::XdevLVertexArray*				m_staticVertexArray;
			xdl::XdevLVertexBuffer* 			m_staticVertexBuffer;

			xdl::xdl_int									m_projViewModelMatrix;
			xdl::xdl_int									m_texture0;
			xdl::xdl_int									m_color;

			xdl::xdl_int									m_gammaid;
			xdl::xdl_int									m_bufferid;
			xdl::xdl_int									m_dftid;
			xdl::xdl_float								m_gamma;
			xdl::xdl_float								m_buffer;
			xdl::xdl_int									m_dft;
			xdl::xdl_int									m_effectid;
			xdl::xdl_int									m_shadowOffsetid;

			xdl::xdl_float								m_dpi;
			xdl::xdl_float								m_scale;
			xdl::xdl_float								m_rescale;
			xdl::xdl_float								m_currentColor[4];
			xdl::xdl_float								m_shadowOffset[2];
			xdl::xdl_uint									m_effectNumber;
			std::vector<XdevLTextLayoutText> 					m_textList;
			std::vector<XdevLTextLayoutText> 					m_staticTextList;
			xdl::xdl_bool										m_staticVertexListUploaded;
			std::map<xdl::xdl_uint, std::vector<XdevLGlyphVertex>> 	m_staticVertexList;
			std::map<xdl::xdl_uint, std::vector<XdevLGlyphVertex>> 	m_dynamicVertexMap;
	};

}

#endif // XDEVL_TEXT_LAYOUT_H
