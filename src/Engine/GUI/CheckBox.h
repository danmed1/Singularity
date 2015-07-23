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

#ifndef CHECK_BOX_H
#define CHECK_BOX_H

#include "Engine/GUI/Widget.h"

class CheckBox : public Widget {
public:
	enum State {
	    CHECKED,
	    UNCHECKED
	};

	typedef xdl::XdevLDelegate<void, const State&, Widget*> OnCheckStateDelegateType;

	CheckBox(const std::wstring& title, xdl::xdl_int x, xdl::xdl_int y, xdl::xdl_int width = 16, xdl::xdl_int height = 16) :
		Widget(this, title, x, y, width, height),
		checked(xdl::xdl_false),
		checkSize(width/4) {}

	virtual void draw() override;

	virtual void onButtonPress(const xdl::XdevLButtonId& buttonid, xdl::xdl_int x, xdl::xdl_int y) override {
		Widget::onButtonPress(buttonid, x, y);
	}

	virtual void onButtonRelease(const xdl::XdevLButtonId& buttonid, xdl::xdl_int x, xdl::xdl_int y) override {
		if(isButtonPressed()) {
			checked = !checked;

			// TODO Shall we call the delegates when the button got released?
			for(auto& onCheckStateDelegate : onCheckStateDelegates) {
				onCheckStateDelegate(checked ? CHECKED : UNCHECKED, this);
			}
		}

		Widget::onButtonRelease(buttonid, x, y);
	}

public:
	/// Bind a delegate for the check event.
	void bindOnCheck(const OnCheckStateDelegateType& delegate) {
		onCheckStateDelegates.push_back(delegate);
	}

	/// Unbind a delegate from the check event.
	void unbindOnCheck(const OnCheckStateDelegateType& delegate) {
		onCheckStateDelegates.remove(delegate);
	}

private:
	xdl::xdl_bool checked;
	xdl::xdl_int checkSize;
	std::list<OnCheckStateDelegateType> onCheckStateDelegates;
};



void CheckBox::draw() {

	const soan::Color& color = getColor();
	const AABB& aabb = getAABB();

	getCanvas()->setCurrentColor(color);
	getCanvas()->drawRect(aabb.x1, aabb.y1, aabb.x2, aabb.y2);

	// Draw the check.
	if(checked) {
		const soan::Color& checkColor = soan::Color(0.0f, 0.0f, 0.0f, 1.0f);
		getCanvas()->setCurrentColor(checkColor);
		getCanvas()->drawRect(aabb.x1 + checkSize, aabb.y1 + checkSize, aabb.x2 - checkSize, aabb.y2 - checkSize);
	}

	if(getBorderSize() > 0) {
		const soan::Color& borderColor = this->getBorderColor();
		getCanvas()->setCurrentColor(borderColor);
		getCanvas()->drawRectLine(aabb.x1, aabb.y1, aabb.x2, aabb.y2);
	}

	getCanvas()->setCurrentColor(getFontColor());
	getCanvas()->drawText(getTitle(), aabb.x1 + aabb.getWidth() + 4, aabb.y1);
}


#endif
