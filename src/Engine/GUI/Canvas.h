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

#ifndef CANVAS_H
#define CANVAS_H

#include <XdevLWindow/XdevLWindow.h>

#include "Engine/Color.h"

namespace soan {

	class Canvas {
		public:
			virtual ~Canvas() {}

			virtual void setCurrentWindow(xdl::IPXdevLWindow window) = 0;
			
			virtual xdl::IPXdevLWindow getWindow() = 0;

			virtual const xdl::xdl_uint& getWidth() const = 0;
			
			virtual const xdl::xdl_uint& getHeight() const = 0;
			
			virtual void makeCurrentWindow() = 0;
			
			virtual void releaseCurrentWindow() = 0;

			virtual void setDimensions(xdl::xdl_uint width, xdl::xdl_uint height) = 0;

			virtual void setCurrentColor(const soan::Color& color) = 0;

			virtual void clearColorBuffer() = 0;

			/// Draw a line using the last point as starting point.
			/**
				This method draws a line strip. The last used point is used as the starting point for the line.
				One has to provide at least two points to draw a line.
			*/
			virtual void drawLine(xdl::xdl_int x1, xdl::xdl_int y1, xdl::xdl_int x2, xdl::xdl_int y2) = 0;

			/// Draw a rectangle.
			virtual void drawRectLine(xdl::xdl_int x1, xdl::xdl_int y1, xdl::xdl_int x2, xdl::xdl_int y2) = 0;
			virtual void drawRect(xdl::xdl_int x1, xdl::xdl_int y1, xdl::xdl_int x2, xdl::xdl_int y2) = 0;

			virtual void drawText(const std::wstring& text, xdl::xdl_float x, xdl::xdl_float y) = 0;

			virtual void render() = 0;
			
			
			virtual void onResized(xdl::xdl_uint width, xdl::xdl_uint height) = 0;
	};

	class CanvasScope {
		public:
			CanvasScope(Canvas* canvas, xdl::IPXdevLWindow window) : _canvas(canvas) {
				_canvas->setCurrentWindow(window);
				_canvas->makeCurrentWindow();
			}
			~CanvasScope() {
				_canvas->releaseCurrentWindow();
			}
			Canvas* _canvas;
	};

}

#endif
