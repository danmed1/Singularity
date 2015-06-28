#ifndef CHECK_BOX_H
#define CHECK_BOX_H

#include "Engine/GUI/Widget.h"

class CheckBox : public Widget {
	public:
		CheckBox(const std::wstring& title, xdl::xdl_int x, xdl::xdl_int y, xdl::xdl_int width = 16, xdl::xdl_int height = 16) :
			Widget(title, x, y, width, height),
			selected(xdl::xdl_false),
			checkSize(width/4) {}

		virtual void draw() override;

		virtual void onButtonPress(const xdl::XdevLButtonId& buttonid, xdl::xdl_int x, xdl::xdl_int y) override {
			Widget::onButtonPress(buttonid, x, y);

			if(isButtonPressed()) {
				selected = !selected;
			}
		}
		/// This is used by the widget scenen system.
		virtual void setWidgetSceneSystem(XdevLQuadTree<int, Widget*>* wss) {
			widgetSceneSystem = wss;
		}

	private:
		xdl::xdl_bool selected;
		xdl::xdl_int checkSize;
};



void CheckBox::draw() {

	const soan::Color& color = getColor();
	const AABB& aabb = getAABB();

	glBegin(GL_TRIANGLE_STRIP);
	glColor4f(color.r, color.g, color.b, color.a);
	glVertex2i(aabb.x1, aabb.y1);
	glVertex2i(aabb.x1, aabb.y2);
	glVertex2i(aabb.x2, aabb.y1);
	glVertex2i(aabb.x2, aabb.y2);
	glEnd();

	if(selected) {
		glBegin(GL_TRIANGLE_STRIP);
		glColor4f(0, 0, 0, 1.0);
		glVertex2i(aabb.x1 + checkSize, aabb.y1 + checkSize);
		glVertex2i(aabb.x1 + checkSize, aabb.y2 - checkSize);
		glVertex2i(aabb.x2 - checkSize, aabb.y1 + checkSize);
		glVertex2i(aabb.x2 - checkSize, aabb.y2 - checkSize);
		glEnd();
	}


	if(getBorderSize() > 0) {
		const soan::Color& borderColor = getBorderColor();
		glLineWidth(getBorderSize());
		glBegin(GL_LINE_STRIP);
		glColor4f(borderColor.r, borderColor.g, borderColor.b, borderColor.a);
		glVertex2i(aabb.x1, aabb.y1);
		glVertex2i(aabb.x2, aabb.y1);
		glVertex2i(aabb.x2, aabb.y2);
		glVertex2i(aabb.x1, aabb.y2);
		glVertex2i(aabb.x1, aabb.y1);
		glEnd();
	}
}


#endif