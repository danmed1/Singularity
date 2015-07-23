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
			Widget(nullptr, L"VBox", 0, 0, 0, 0) {
		}

		void addChild(Widget* node) {
			node->setParent(this);
			children.push_back(node);
		}

		void removeChild(Widget* node) {
			std::list<Widget*>::iterator it = std::find(children.begin(), children.end(), node);
			if(it != children.end()) {
				(*it)->setParent(nullptr);
				children.erase(it);
			}
		}

		void update() {
			int height = 0;
			for(auto& widget : children) {
				const AABB& aabb = widget->getAABB();
				height += aabb.getHeight();
			}
			AABB newaabb = getAABB();
			newaabb.y2 = newaabb.y1 + height;
			setAABB(newaabb);
		}

		virtual void draw() override {
			for(auto& widget : children) {
				widget->draw();
			}
		}

	private:
		std::list<Widget*> children;
};

#endif
