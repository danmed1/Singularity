#ifndef GUI_WIDGETS_H
#define GUI_WIDGETS_H

#include "Engine/Color.h"

/**
 * @class AABB
 * @brief Axis Aligned Bounding Box
 */
class AABB {
public:
	AABB() : x1(0), y1(0), x2(0), y2(0) {}

	AABB(xdl::xdl_int _x1, xdl::xdl_int _y1, xdl::xdl_int _x2, xdl::xdl_int _y2) : x1(_x1), y1(_y1), x2(_x2), y2(_y2) {}

	xdl::xdl_bool isPointInside(xdl::xdl_int x, xdl::xdl_int y) {
		return ((x1 <= x) && (x <= x2) && (y1 <= y) && (y <= y2));
	}
	xdl::xdl_int getWidth() const {
		return (x2-x1);
	}
	xdl::xdl_int getHeight() const {
		return (y2-y1);
	}
	xdl::xdl_int x1, y1, x2, y2;
};

/**
 * @class Widget
 * @brief A Widget which is a base class for most widget types.
 */
class Widget {
public:
	typedef xdl::XdevLDelegate<void, const xdl::XdevLButtonId&, const Widget&> WidgetButtonPressedDelegate;
	typedef xdl::XdevLDelegate<void, const xdl::XdevLButtonId&, const Widget&> WidgetButtonReleasedDelegate;

	enum AnchorPosition {
	    CENTER,
	    TOP_LEFT,
	    TOP_RIGHT,
	    BOTTOM_LEFT,
	    BOTTOM_RIGHT
	};

	enum ColorRegions {
	    STANDARD,
	    POINTER_HOVER,
	    BUTTON_PRESSED
	};

	explicit Widget(xdl::xdl_int x, xdl::xdl_int y, xdl::xdl_int width, xdl::xdl_int height) :
		aabb(x, y, x + width, y + height),
		currentColorLevel(0),
		mouseHovers(xdl::xdl_false),
		buttonPressed(xdl::xdl_false),
		highLightOnMouseHover(xdl::xdl_false),
		changeOnButtonPress(xdl::xdl_false) {

		color[0] = soan::Color(0.53f, 0.52f, 0.50f, 1.0f);
		color[1] = soan::Color(0.73f, 0.72f, 0.70f, 1.0f);
		color[2] = soan::Color(1.0f, 0.0f, 0.0f, 1.0f);
	}

	/// Method to draw the widget.
	virtual void draw() = 0;

	/// Returns the AABB of this widget.
	const AABB& getAABB() const {
		return aabb;
	}

	/// Sets the current overall color of the widget.
	void useColor(xdl::xdl_int idx) {
		currentColorLevel = idx;
	}

	/// Returns the current overall color of the widget.
	const soan::Color& getColor() {
		return color[currentColorLevel];
	}

	/// Mouse got moved.
	virtual void onMouseMove(xdl::xdl_int x, xdl::xdl_int y) {
		mouseHovers = aabb.isPointInside(x, y);

		if(mouseHovers && !buttonPressed) {
			if(highLightOnMouseHover) {
				useColor(POINTER_HOVER);
			}
		} else if(mouseHovers && buttonPressed) {
			if(changeOnButtonPress) {
				useColor(BUTTON_PRESSED);
			}
		} else {
			useColor(STANDARD);
		}
	}

	/// A button got pressed.
	virtual void onButtonPress(const xdl::XdevLButtonId& buttonid, xdl::xdl_int x, xdl::xdl_int y) {
		buttonPressed = aabb.isPointInside(x, y);

		// Call here some delegates?
		if(buttonPressed) {
			if(changeOnButtonPress) {
				useColor(BUTTON_PRESSED);
			}
		}

		// Call delegates if there a one.
		for(auto& delegate : buttonPressedDelegates) {
			delegate(buttonid, *this);
		}
	}

	/// A button got released.
	virtual void onButtonRelease(const xdl::XdevLButtonId& buttonid, xdl::xdl_int x, xdl::xdl_int y) {
		buttonPressed = aabb.isPointInside(x, y);

		buttonPressed = xdl::xdl_false;
		if(mouseHovers) {
			if(highLightOnMouseHover) {
				useColor(POINTER_HOVER);
			}
		} else {
			useColor(STANDARD);
		}

		// Call delegates if there a one.
		for(auto& delegate : buttonReleasedDelegates) {
			delegate(buttonid, *this);
		}
	}

	xdl::xdl_bool isButtonPressed() {
		return buttonPressed;
	}

	xdl::xdl_bool doesMouseHover() {
		return mouseHovers;
	}

	void setHighLightOnMouseHover(xdl::xdl_bool state) {
		highLightOnMouseHover = state;
	}

	void setChangeColorOnButtonPress(xdl::xdl_bool state) {
		changeOnButtonPress = state;
	}
private:

	AABB aabb;

	xdl::xdl_uint currentColorLevel;
	soan::Color color[4];

	xdl::xdl_bool mouseHovers;
	xdl::xdl_bool buttonPressed;
	xdl::xdl_bool highLightOnMouseHover;
	xdl::xdl_bool changeOnButtonPress;

	std::vector<WidgetButtonPressedDelegate> buttonPressedDelegates;
	std::vector<WidgetButtonReleasedDelegate> buttonReleasedDelegates;

};

class Button : public Widget {
public:
	Button(const std::wstring& title, xdl::xdl_int x, xdl::xdl_int y, xdl::xdl_int width, xdl::xdl_int height) :
		Widget(x, y, width, height),
		text(title) {

		// Change color when mouse hovers.
		setHighLightOnMouseHover(xdl::xdl_true);

		// Change color when mouse button pressed.
		setChangeColorOnButtonPress(xdl::xdl_true);
	}

	const std::wstring& title() const {
		return text;
	}

	virtual void draw() override;

private:
	std::wstring text;
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
}


class CheckBox : public Widget {
public:
	CheckBox(const std::wstring& title, xdl::xdl_int x, xdl::xdl_int y, xdl::xdl_int width = 16, xdl::xdl_int height = 16) :
		Widget(x, y, width, height),
		text(title),
		selected(xdl::xdl_false),
		checkSize(width/4) {}

	const std::wstring& title() const {
		return text;
	}

	virtual void draw() override;

	virtual void onButtonPress(const xdl::XdevLButtonId& buttonid, xdl::xdl_int x, xdl::xdl_int y) override {
		Widget::onButtonPress(buttonid, x, y);

		if(isButtonPressed()) {
			selected = !selected;
		}
	}

private:
	std::wstring text;
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
}



class ComboBox : public Widget {
public:
	ComboBox(xdl::xdl_int x, xdl::xdl_int y, xdl::xdl_int width, xdl::xdl_int height) :
		Widget(x, y, width, height),
		barCursorY(y) {

		// Change color when mouse hovers.
		setHighLightOnMouseHover(xdl::xdl_true);

		// Change color when mouse button pressed.
		setChangeColorOnButtonPress(xdl::xdl_true);
	}

	virtual void draw() override;

	virtual void onButtonPress(const xdl::XdevLButtonId& buttonid, xdl::xdl_int x, xdl::xdl_int y) override {
		Widget::onButtonPress(buttonid, x, y);
		
		isActivated = !isActivated;
	}

	void addItem(const std::wstring& title) {
		barCursorY += getAABB().getHeight();
		m_list.push_back(new Button(title, getAABB().x1, barCursorY, getAABB().getWidth(), getAABB().getHeight()));
	}

private:
	std::wstring text;
	std::vector<Widget*> m_list;
	xdl::xdl_int barCursorY;
	xdl::xdl_bool isActivated;
};



void ComboBox::draw() {

	{
		glBegin(GL_TRIANGLE_STRIP);

		const soan::Color& color = getColor();
		const AABB& aabb = getAABB();
		glColor4f(color.r, color.g, color.b, color.a);

		glVertex2i(aabb.x1, aabb.y1);
		glVertex2i(aabb.x1, aabb.y2);
		glVertex2i(aabb.x2, aabb.y1);
		glVertex2i(aabb.x2, aabb.y2);

		glEnd();
	}

	if(isActivated) {
		if(m_list.size() > 0) {
			glBegin(GL_TRIANGLE_STRIP);

			for(auto& item : m_list) {
				const soan::Color& color = item->getColor();
				const AABB& aabb = item->getAABB();
				glColor4f(color.r, color.g, color.b, color.a);

				glVertex2i(aabb.x1, aabb.y1);
				glVertex2i(aabb.x1, aabb.y2);
				glVertex2i(aabb.x2, aabb.y1);
				glVertex2i(aabb.x2, aabb.y2);
			}

			glEnd();
		}
	}
}

class MenuBar : public Widget {
public:
	MenuBar(xdl::xdl_int x, xdl::xdl_int y, xdl::xdl_int width, xdl::xdl_int height) :
		Widget(x, y, width, height),
		barCursorX(x),
		barCursorY(y) {}

	virtual void draw() override;

	void addMenu(const std::wstring& title) {
		barWidgets.push_back(new Button(title, barCursorX, barCursorY, 50, getAABB().getHeight()));
		barCursorX+= 50;
	}

	virtual void onButtonPress(const xdl::XdevLButtonId& buttonid, xdl::xdl_int x, xdl::xdl_int y) override {
		Widget::onButtonPress(buttonid, x, y);

		if(isButtonPressed()) {

		}
	}

	std::vector<Widget*>& getWidgets() {
		return barWidgets;
	}

private:
	std::vector<Widget*> barWidgets;
	xdl::xdl_int barCursorX;
	xdl::xdl_int barCursorY;
};


void MenuBar::draw() {
	const soan::Color& color = getColor();
	const AABB& aabb = getAABB();
	glBegin(GL_TRIANGLE_STRIP);
	glColor4f(color.r, color.g, color.b, color.a);
	glVertex2i(aabb.x1, aabb.y1);
	glVertex2i(aabb.x1, aabb.y2);
	glVertex2i(aabb.x2, aabb.y1);
	glVertex2i(aabb.x2, aabb.y2);
	glEnd();

	for(auto& widget : barWidgets) {
		widget->draw();
	}

}



void convertWidgetAABBToRelative(xdl::xdl_uint anchor, const AABB& aabb, xdl::xdl_float width, xdl::xdl_float height, xdl::xdl_float& x, xdl::xdl_float& y) {
	x = -1.0f;
	y = 1.0f;
	xdl::xdl_float unit = (2.0/width);
	switch(anchor) {
		case Widget::CENTER: {
			x += unit*(xdl::xdl_float)(aabb.x2 - aabb.x1)/2.0f;
			y -= unit*(xdl::xdl_float)(aabb.y2 - aabb.y1)/2.0f;
		}
		break;
		case Widget::BOTTOM_LEFT: {
			x += unit*(xdl::xdl_float)aabb.x1;
			y -= unit*(xdl::xdl_float)aabb.y2;
		}
		break;

	}

}


#endif
