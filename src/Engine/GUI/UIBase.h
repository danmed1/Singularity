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

#ifndef UI_BASE_H
#define UI_BASE_H

#include <XdevLTypes.h>
#include <vector>

namespace gui {

	struct Rect {
		Rect(xdl::xdl_float _x, xdl::xdl_float _y, xdl::xdl_float _width, xdl::xdl_float _height) :
			x(_x), y(_y), width(_width), height(_height) {}

		xdl::xdl_float x;
		xdl::xdl_float y;
		xdl::xdl_float width;
		xdl::xdl_float height;
	};

	class UIBase {
		public:
			UIBase();
			virtual ~UIBase();
			void drawRect(xdl::xdl_float x, xdl::xdl_float y, xdl::xdl_float width, xdl::xdl_float height);
		private:
			std::vector<Rect> m_rectArray;
	};

}

#endif // UI_BASE_H
