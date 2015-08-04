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

#ifndef VBOX_H
#define VBOX_H

#include <Engine/GUI/Node.h>

class VBox : public Widget {
	public:
		VBox(const AnchorPosition& anchorPosition, const ResizePolicy& verticalResizePolicy, const ResizePolicy& horizontalResizePolicy ) :
			Widget(nullptr, L"VBox", 100, 200, 0, 0),
			gotDirty(xdl::xdl_true) {
		}

		virtual void update() override {
			int height = 0;

//			AABB vboxaabb(getAABB());

			for(auto& widget : getChildren()) {
				
//				AABB tmp(widget->getAABB());
//				
//				tmp.x1 = vboxaabb.x1;
//				tmp.x2 = vboxaabb.x1 + widget->getAABB().getWidth();
//				tmp.y1 = vboxaabb.y1 + height;
//				tmp.y2 = vboxaabb.y1 + widget->getAABB().getHeight() + height;
//
//				height -= widget->getAABB().getHeight();
//
//				widget->setAABB(tmp);
				widget->update();
				
			}
			gotDirty = xdl::xdl_false;

		}

		virtual void draw() override {
			if(gotDirty) {
				update();
			}

			for(auto& widget : getChildren()) {
				widget->draw();
			}
		}

	private:
	xdl::xdl_bool gotDirty;
};

#endif
