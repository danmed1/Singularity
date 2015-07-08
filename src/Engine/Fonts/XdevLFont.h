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

#ifndef XDEVL_FONT_H
#define XDEVL_FONT_H

#include <XdevLOpenGL/XdevLTexture.h>

namespace soan {


	/**
		@struct XdevLFontVertex
		@brief The structure of the vertex for a font triangle.
	*/
	struct XdevLGlyphVertex {
		xdl::xdl_float x,y;		/// Position data.
		xdl::xdl_uint8 r,g,b,a;	/// Color data.
		xdl::xdl_float s, t;	/// Texture coordinate data.
	};

	/**
		@struct XdevLGlyphMetric
		@brief A structure that describes the metric of a glyph.
		
          Glyph metrics:
          --------------
         
                                xmin                     xmax
                                 |                         |
                                 |<-------- width -------->|
                                 |                         |
                       |         +-------------------------+----------------- ymax
                       |         |    aaaaaaaaa   aaaaa    |     ^        ^
                       |         |   a:::::::::aaa::::a    |     |        |
                       |         |  a:::::::::::::::::a    |     |        |
                       |         | a::::::aaaaa::::::aa    |     |        |
                       |         | a:::::a     a:::::a     |     |        |
             offset_x -|-------->| a:::::a     a:::::a     |  offset_y    |
                       |         | a:::::a     a:::::a     |     |        |
                       |         | a::::::a    a:::::a     |     |        |
                       |         | a:::::::aaaaa:::::a     |     |        |
                       |         |  a::::::::::::::::a     |     |      height
                       |         |   aa::::::::::::::a     |     |        |
           baseline ---*---------|---- aaaaaaaa::::::a-----*--------      |
                     / |         |             a:::::a     |              |
              origin   |         | aaaaaa      a:::::a     |              |
                       |         | a:::::aa   aa:::::a     |              |
                       |         |  a::::::aaa:::::::a     |              |
                       |         |   aa:::::::::::::a      |              |
                       |         |     aaa::::::aaa        |              |
                       |         |         aaaaaa          |              v
                       |         +-------------------------+----------------- ymin
                       |                                   |
                       |------------- advance_x ---------->|
	*/
	struct XdevLGlyphMetric {
		XdevLGlyphMetric() :
			tid(0),
			id(0),
			left(0.0f),
			top(0.0f),
			width(0.0f),
			height(0.0f),
			horizontalLayoutAdvance(0.0f),
			verticalLayoutAdvance(0.0f),
			horizontalLayoutBearingX(0.0f),
			horizontalLayoutBearingY(0.0f),
			verticalLayoutBearingX(0.0f),
			verticalLayoutBearingY(0.0f)
        {
			memset(vertices, 0, sizeof(XdevLGlyphVertex)*4);
		}
		uint32_t			tid;
		uint32_t 			id;
		xdl::xdl_float 		left;
		xdl::xdl_float 		top;
		xdl::xdl_float 		width;
		xdl::xdl_float 		height;
		xdl::xdl_float 		horizontalLayoutAdvance;
		xdl::xdl_float 		verticalLayoutAdvance;
		xdl::xdl_float 		horizontalLayoutBearingX;
		xdl::xdl_float 		horizontalLayoutBearingY;
		xdl::xdl_float 		verticalLayoutBearingX;
		xdl::xdl_float 		verticalLayoutBearingY;

		XdevLGlyphVertex 	vertices[4];
	};

	/**
		@class XdevLFont
		@brief This is an interface for font support.
	*/
	class XdevLFont {
	public:
		virtual ~XdevLFont() {}

		/// The function pointer that creates a texture out of an image.
		typedef xdl::XdevLTexture* (*createTextureFromFileCallbackFunction)(const xdl::xdl_char* imageFileName);

		/// Create font from texture.
		virtual xdl::xdl_int createFontFromTexture(const xdl::xdl_char* fontInfoFilename, xdl::XdevLTexture* texture) = 0;

		/// Returns a glphy's metric information.
		virtual XdevLGlyphMetric& getGlyphMetric(xdl::xdl_uint32 unicode) = 0;

		/// Returns the font size in pixels.
		/**
			This font size is the maximum with in horizontal direction. The glphys width might be different than this size.
		*/
		virtual xdl::xdl_float getFontSize() = 0;

		/// Returns the new line size.
		virtual xdl::xdl_float getNewLineSize() = 0;

		/// Returns the number of textures used by this font.
		virtual xdl::xdl_uint getNumberOfTextures() = 0;

		/// Returns a specific texture used for this font.
		virtual xdl::XdevLTexture* getTexture(xdl::xdl_uint idx) = 0;

		/// Sets the function which imports a image and creates a texture.
		virtual void setCreateTextureCallback(createTextureFromFileCallbackFunction function) = 0;
	};

}

#endif // XDEVL_FONT_H
