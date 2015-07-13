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

#ifndef BUTTON_H
#define BUTTON_H

class Button : public Widget {
	public:
		Button(const std::wstring& title, xdl::xdl_int x, xdl::xdl_int y, xdl::xdl_int width, xdl::xdl_int height) :
			Widget(title, x, y, width, height) {

			// Change color when mouse hovers.
			setHighLightOnMouseHover(xdl::xdl_true);

			// Change color when mouse button pressed.
			setChangeColorOnButtonPress(xdl::xdl_true);
		}

		/// This is used by the widget scenen system.
		virtual void setWidgetSceneSystem(XdevLQuadTree<int, Widget*>* wss) {
			eventGrid = wss;
		}


		virtual void draw() override;
};



void Button::draw() {
	const soan::Color& color = getColor();
	const AABB& aabb = getAABB();

	getCanvas()->setCurrentColor(color);
	getCanvas()->drawRect(aabb.x1, aabb.y1, aabb.x2, aabb.y2);

	if(getBorderSize() > 0) {
		const soan::Color& borderColor = this->getBorderColor();
		getCanvas()->setCurrentColor(borderColor);
		getCanvas()->drawRectLine(aabb.x1, aabb.y1, aabb.x2, aabb.y2);
	}

	getCanvas()->setCurrentColor(getFontColor());
	getCanvas()->drawText(getTitle(), aabb.x1, aabb.y1 + aabb.getHeight()/2);

}

#endif
