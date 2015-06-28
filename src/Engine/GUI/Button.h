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
		widgetSceneSystem = wss;
	}


	virtual void draw() override;
};



void Button::draw() {

	glBegin(GL_TRIANGLE_STRIP);

	const soan::Color& color = getColor();
	const AABB& aabb = getAABB();
	glColor4f(color.r, color.g, color.b, color.a);

	glVertex2i(aabb.x1, aabb.y1);
	glVertex2i(aabb.x1, aabb.y2);
	glVertex2i(aabb.x2, aabb.y1);
	glVertex2i(aabb.x2, aabb.y2);

	glEnd();

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
