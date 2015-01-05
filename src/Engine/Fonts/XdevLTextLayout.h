/*
	XdevL eXtended DEVice Library.

	Copyright © 2005-2015 Cengiz Terzibas

	This library is free software; you can redistribute it and/or modify it under the
	terms of the GNU Lesser General Public License as published by the Free Software
	Foundation; either version 2.1 of the License, or (at your option) any later version.
	This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
	without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
	See the GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License along with
	this library; if not, write to the Free Software Foundation, Inc., 59 Temple Place,
	Suite 330, Boston, MA 02111-1307 USA

	I would appreciate if you report all bugs to: cengiz@terzibas.de
*/

#ifndef XDEVL_TEXT_LAYOUT_H
#define XDEVL_TEXT_LAYOUT_H

#include <Engine/Fonts/XdevLFont.h>
#include <string>

namespace soan {

	/**
		@struct XdevLTextLayoutText
		@brief A structure holds data for text rendering.
	*/
	struct XdevLTextLayoutText {
		/// The text to render.
		std::wstring text;
		
		/// The start position in x direction.
		xdl::xdl_float x;
		
		/// The start position in y direction.
		xdl::xdl_float y;
		
		/// The color of the whole text.
		xdl::xdl_float color[4];
		
		/// The scale factor of the whole text.
		xdl::xdl_float scale;
	};

	/**
		@class XdevLTextLayout
		@brief A interface for a text layout engine.
	*/
	class XdevLTextLayout {
		public:
			virtual ~XdevLTextLayout() {}

			/// Initialize the layout engine.
			virtual xdl::xdl_int init(XdevLFont* font) = 0;

			/// Add dynamic text.
			/**
				A dyanmic text is a text which change the number of letters and/or letters itself. 
				A vertex buffer is created and continuously updated.
			*/
			virtual void addDynamicText(const std::wstring& text, xdl::xdl_float x, xdl::xdl_float y) = 0;

			/// Sets the scale factor.
			virtual void setScale(xdl::xdl_float scale) = 0;

			/// Sets the DPI.
			virtual void setDPI(xdl::xdl_float) = 0;

			/// Sets the current color.
			virtual void setColor(xdl::xdl_float r, xdl::xdl_float g, xdl::xdl_float b) = 0;

			/// Sets the font rendering effect.
			virtual void setEffect(xdl::xdl_uint effectNumber) = 0;

			/// Sets the shadow offset parameter.
			virtual void setShadowOffset(xdl::xdl_float xOffset, xdl::xdl_float yOffset) = 0;

			/// Draws everything.
			virtual void render() = 0;
	};

}

#endif
