#ifndef GUI_WIDGETS_H
#define GUI_WIDGETS_H

#include "Engine/Color.h"
#include "Engine/GUI/WidgetSceneSystemUtils.h"


/**
 * @class Widget
 * @brief A Widget which is a base class for most widget types.
 */
class Widget {
	public:
		typedef xdl::XdevLDelegate<void, const xdl::XdevLButtonId&, const Widget&> WidgetButtonPressedDelegate;
		typedef xdl::XdevLDelegate<void, const xdl::XdevLButtonId&, const Widget&> WidgetButtonReleasedDelegate;

		typedef xdl::XdevLDelegate<void, Widget*> OnClickedDelegate;
		typedef xdl::XdevLDelegate<void, Widget*> OnPointerHoverDelegate;

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

		Widget(const std::wstring& title, xdl::xdl_int x, xdl::xdl_int y, xdl::xdl_int width, xdl::xdl_int height) :
			text(title),
			aabb(x, y, x + width, y + height),
			currentColorLevel(0),
			borderColor(0.2f, 0.2f, 0.2f, 1.0f),
			borderSize(1),
			mouseHovers(xdl::xdl_false),
			buttonPressed(xdl::xdl_false),
			highLightOnMouseHover(xdl::xdl_false),
			changeOnButtonPress(xdl::xdl_false),
			buttonPressedTimeStamp(0),
			buttonReleaseTimeStamp(0) {

			color[0] = soan::Color(0.53f, 0.52f, 0.50f, 1.0f);
			color[1] = soan::Color(0.73f, 0.72f, 0.70f, 1.0f);
			color[2] = soan::Color(1.0f, 0.0f, 0.0f, 1.0f);
		}

		Widget(xdl::xdl_int x, xdl::xdl_int y, xdl::xdl_int width, xdl::xdl_int height) :
			Widget(L"Default", x, y, width, height) {
		}

		/// Method to draw the widget.
		virtual void draw() = 0;

		/// Returns the AABB of this widget.
		const AABB& getAABB() const {
			return aabb;
		}

		/// Sets the current overall color of the widget.
		// TODO This is at the moment quite hacky and only for testing purpose. Will change later.
		void useColor(xdl::xdl_int idx) {
			currentColorLevel = idx;
		}

		/// Returns the current overall color of the widget.
		// TODO This is at the moment quite hacky and only for testing purpose. Will change later.
		const soan::Color& getColor() {
			return color[currentColorLevel];
		}

		/// The mouse got moved, let's do what we have to do with this information.
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
				buttonPressedTimeStamp = timer.getTime64();
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
			if(aabb.isPointInside(x, y)) {

				buttonReleaseTimeStamp = timer.getTime64();
				if((buttonReleaseTimeStamp - buttonPressedTimeStamp) < 350000) {
					for(auto& delegate : onClickedDelegates) {
						if(delegate.isValid()) {
							delegate(this);
						} else {
							// TODO Do something really awesome here :D
							std::cerr << "No delegate assigned to OnClicked event." << std::endl;
						}
					}

				}

				// Yeah well, if the button got released the button is not pressed anymore right.
				buttonPressed = xdl::xdl_false;

				// While the button was released, does the pointer still hover over this widget?
				if(mouseHovers) {

					// OK it still hovers over this widget but do we have to do something about the
					// hovering color behavior?
					if(highLightOnMouseHover) {
						useColor(POINTER_HOVER);
					}
				} else {
					// Definitely switch back the background color.
					useColor(STANDARD);
				}

				// Call delegates if there a one.
				for(auto& delegate : buttonReleasedDelegates) {
					delegate(buttonid, *this);
				}
			}
		}

		/// Is the left mouse button pressed?
		xdl::xdl_bool isButtonPressed() {
			return buttonPressed;
		}

		/// Does the pointer hover over this widget?
		xdl::xdl_bool doesMouseHover() {
			return mouseHovers;
		}

		/// Set the state if the widget should change it's state while pointer hovers over it.
		void setHighLightOnMouseHover(xdl::xdl_bool state) {
			highLightOnMouseHover = state;
		}

		/// Set the state if the widget should change it's state when the left mouse button is pressed.
		void setChangeColorOnButtonPress(xdl::xdl_bool state) {
			changeOnButtonPress = state;
		}

		/// This is used by the widget scenen system.
		virtual void setWidgetSceneSystem(XdevLQuadTree<int, Widget*>* wss) = 0;

		xdl::xdl_uint getBorderSize() const  {
			return borderSize;
		}

		void setBorderSize(xdl::xdl_uint size) {
			borderSize = size;
		}

		const soan::Color& getBorderColor() const {
			return borderColor;
		}

		const std::wstring& getTitle() const {
			return text;
		}

	public:

		//
		// Delegates handling
		//

		void bindOnClicked(const OnClickedDelegate& delegate) {
			onClickedDelegates.push_back(delegate);
		}

		void unbindOnClicked(const OnClickedDelegate& delegate) {
			auto it = std::find(onClickedDelegates.begin(), onClickedDelegates.end(), delegate);
			if(it != onClickedDelegates.end()) {
				onClickedDelegates.erase(it);
			}
		}

		void bindOnPointerHover(const OnPointerHoverDelegate& delegate) {
			onPointerHoverDelegates.push_back(delegate);
		}

		void unbindOnPointerHover(const OnClickedDelegate& delegate) {
			auto it = std::find(onPointerHoverDelegates.begin(), onPointerHoverDelegates.end(), delegate);
			if(it != onPointerHoverDelegates.end()) {
				onPointerHoverDelegates.erase(it);
			}
		}
	protected:

		std::wstring text;

	private:
		AABB aabb;
		xdl::XdevLTimer timer;
		xdl::xdl_uint currentColorLevel;
		soan::Color color[4];
		soan::Color borderColor;
		xdl::xdl_uint borderSize;
		xdl::xdl_bool mouseHovers;
		xdl::xdl_bool buttonPressed;
		xdl::xdl_bool highLightOnMouseHover;
		xdl::xdl_bool changeOnButtonPress;

		xdl::xdl_uint64 buttonPressedTimeStamp;
		xdl::xdl_uint64 buttonReleaseTimeStamp;

		std::vector<WidgetButtonPressedDelegate> buttonPressedDelegates;
		std::vector<WidgetButtonReleasedDelegate> buttonReleasedDelegates;
	public:
		XdevLQuadTree<int, Widget*>* widgetSceneSystem;

	private:
		std::vector<OnClickedDelegate> onClickedDelegates;
		std::vector<OnPointerHoverDelegate> onPointerHoverDelegates;

};

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

/**
	@class ContextMenu
	@brief A context menu class
*/
class ContextMenu : public Widget {
	public:
		typedef xdl::XdevLDelegate<void, Widget*> OnItemSelectedDelegateType;

		ContextMenu(xdl::xdl_int x, xdl::xdl_int y, xdl::xdl_int width, xdl::xdl_int height) :
			Widget(x, y, width, height),
			barCursorY(y),
			isActivated(xdl::xdl_false),
			currentSelectedIndex(0),
			currentSelectedItem(nullptr) {

			// Change color when mouse hovers.
			setHighLightOnMouseHover(xdl::xdl_true);

			// Change color when mouse button is pressed.
			setChangeColorOnButtonPress(xdl::xdl_true);

			// Create a delegate that handles user selections. If the user clicks a combo box it will show
			// the contents. Now we have to know which one of those contents will be clicked. We use
			// this delegate to tell the items in the combo box that we are interested if they get clicked.
			selectedDelegate = Widget::OnClickedDelegate::Create<ContextMenu, &ContextMenu::onSelectedClicked>(this);
		}

		/// Handle button press events here.
		virtual void onButtonPress(const xdl::XdevLButtonId& buttonid, xdl::xdl_int x, xdl::xdl_int y) override {

			// Don't touch this. Always call this first in a derived class from the Widget class.
			Widget::onButtonPress(buttonid, x, y);

			if(isButtonPressed()) {
				isActivated = !isActivated;

				// Did the user activated the combo box pressing the mouse button?
				if(isActivated) {

					// Yes, so we have to add widgets that is used as items of the ComboBox into the event grid
					// and we assigned a delegate that will inform us when the user selects an item.

					// First assign the delegate that will handles selection events.
					for(auto& widget : combBoxItemWidgetList) {
						widget->bindOnClicked(selectedDelegate);
					}

					// Now activate all widgets on the event grid.
					widgetSceneSystem->insertObjectAll(combBoxItemWidgetList);
				} else {

					// No that means the ComboBox got deactivated. First we remove all delegates that handles
					// selection events.
					for(auto& widget : combBoxItemWidgetList) {
						widget->unbindOnClicked(selectedDelegate);
					}

					// And then we deactivate all widgets from the event grid.
					widgetSceneSystem->removeObjectAll(combBoxItemWidgetList);
				}
			}
		}

		void onSelectedClicked(Widget* widget) {
			// If this method is called the user selected an item in the ComboBox list. We have to deactivate the ComboBox
			// remove all widgets from the event grid and unbind the delegates to handle selection events from the user.

			isActivated = xdl::xdl_false;
			currentSelectedItem = widget;

			for(auto& widget : combBoxItemWidgetList) {
				widget->unbindOnClicked(selectedDelegate);
			}
			widgetSceneSystem->removeObjectAll(combBoxItemWidgetList);

			for(auto& delegate : onItemSelectedDelegates) {
				delegate(widget);
			}
		}

		/// Add one item into the CheckBox with specific delegate.
		void addItem(const std::wstring& title, const OnClickedDelegate& delegate) {
			// TODO Yeah well this all is still hacky so make it better later cengiz :D
			barCursorY += getAABB().getHeight();
			Button* button = new Button(title, getAABB().x1, barCursorY, getAABB().getWidth(), getAABB().getHeight());
			button->bindOnClicked(delegate);
			combBoxItemWidgetList.push_back(button);
		}

		/// Add one item into the CheckBox using the CheckBox delegate.
		void addItem(const std::wstring& title) {
			// TODO Yeah well this all is still hacky so make it better later cengiz :D
			barCursorY += getAABB().getHeight();
			Button* button = new Button(title, getAABB().x1, barCursorY, getAABB().getWidth(), getAABB().getHeight());
			combBoxItemWidgetList.push_back(button);
		}

		std::vector<Widget*>& getWidgets() {
			return combBoxItemWidgetList;
		}

		xdl::xdl_int getCurrentSelectedIndex() const {
			return currentSelectedIndex;
		}


		Widget* getCurrentSelectedItem() const {
			return currentSelectedItem;
		}

		virtual void draw() override;

		virtual void setWidgetSceneSystem(XdevLQuadTree<int, Widget*>* wss) {
			widgetSceneSystem = wss;
			
//			for(auto& widget : combBoxItemWidgetList) {
//				widgetSceneSystem->insertObject(widget);
//			}
		}

	public:

		void bindOnItemSelected(const OnItemSelectedDelegateType& delegate) {
			onItemSelectedDelegates.push_back(delegate);
		}

		void unbindOnItemSelected(const OnClickedDelegate& delegate) {
			auto it = std::find(onItemSelectedDelegates.begin(), onItemSelectedDelegates.end(), delegate);
			if(it != onItemSelectedDelegates.end()) {
				onItemSelectedDelegates.erase(it);
			}
		}

	private:
		std::vector<Widget*> combBoxItemWidgetList;
		xdl::xdl_int barCursorY;
		xdl::xdl_bool isActivated;
		xdl::xdl_int currentSelectedIndex;
		Widget* currentSelectedItem;
		Widget::OnClickedDelegate selectedDelegate;
		std::vector<OnItemSelectedDelegateType> onItemSelectedDelegates;
};



void ContextMenu::draw() {
	const AABB& aabb = getAABB();
	{
		glBegin(GL_TRIANGLE_STRIP);

		const soan::Color& color = getColor();
		glColor4f(color.r, color.g, color.b, color.a);

		glVertex2i(aabb.x1, aabb.y1);
		glVertex2i(aabb.x1, aabb.y2);
		glVertex2i(aabb.x2, aabb.y1);
		glVertex2i(aabb.x2, aabb.y2);

		glEnd();

	}

	if(isActivated) {
		if(combBoxItemWidgetList.size() > 0) {

			for(auto& item : combBoxItemWidgetList) {
				glBegin(GL_TRIANGLE_STRIP);
				const soan::Color& color = item->getColor();
				const AABB& itemaabb = item->getAABB();
				glColor4f(color.r, color.g, color.b, color.a);

				glVertex2i(itemaabb.x1, itemaabb.y1);
				glVertex2i(itemaabb.x1, itemaabb.y2);
				glVertex2i(itemaabb.x2, itemaabb.y1);
				glVertex2i(itemaabb.x2, itemaabb.y2);
				glEnd();
			}

		}
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


/**
	@class ComboBox
	@brief A combobox class
*/
class ComboBox : public Widget {
	public:
		typedef xdl::XdevLDelegate<void, Widget*> OnItemSelectedDelegateType;

		ComboBox(xdl::xdl_int x, xdl::xdl_int y, xdl::xdl_int width, xdl::xdl_int height) :
			Widget(x, y, width, height),
			barCursorY(y),
			isActivated(xdl::xdl_false),
			currentSelectedIndex(0),
			currentSelectedItem(nullptr) {

			// Change color when mouse hovers.
			setHighLightOnMouseHover(xdl::xdl_true);

			// Change color when mouse button is pressed.
			setChangeColorOnButtonPress(xdl::xdl_true);

			// Create a delegate that handles user selections. If the user clicks a combo box it will show
			// the contents. Now we have to know which one of those contents will be clicked. We use
			// this delegate to tell the items in the combo box that we are interested if they get clicked.
			selectedDelegate = Widget::OnClickedDelegate::Create<ComboBox, &ComboBox::onSelectedClicked>(this);
		}

		/// Handle button press events here.
		virtual void onButtonPress(const xdl::XdevLButtonId& buttonid, xdl::xdl_int x, xdl::xdl_int y) override {

			// Don't touch this. Always call this first in a derived class from the Widget class.
			Widget::onButtonPress(buttonid, x, y);

			if(isButtonPressed()) {
				isActivated = !isActivated;

				// Did the user activated the combo box pressing the mouse button?
				if(isActivated) {

					// Yes, so we have to add widgets that is used as items of the ComboBox into the event grid
					// and we assigned a delegate that will inform us when the user selects an item.

					// First assign the delegate that will handles selection events.
					for(auto& widget : combBoxItemWidgetList) {
						widget->bindOnClicked(selectedDelegate);
					}

					// Now activate all widgets on the event grid.
					widgetSceneSystem->insertObjectAll(combBoxItemWidgetList);
				} else {

					// No that means the ComboBox got deactivated. First we remove all delegates that handles
					// selection events.
					for(auto& widget : combBoxItemWidgetList) {
						widget->unbindOnClicked(selectedDelegate);
					}

					// And then we deactivate all widgets from the event grid.
					widgetSceneSystem->removeObjectAll(combBoxItemWidgetList);
				}
			}
		}

		void onSelectedClicked(Widget* widget) {
			// If this method is called the user selected an item in the ComboBox list. We have to deactivate the ComboBox
			// remove all widgets from the event grid and unbind the delegates to handle selection events from the user.

			isActivated = xdl::xdl_false;
			currentSelectedItem = widget;

			for(auto& widget : combBoxItemWidgetList) {
				widget->unbindOnClicked(selectedDelegate);
			}
			widgetSceneSystem->removeObjectAll(combBoxItemWidgetList);

			for(auto& delegate : onItemSelectedDelegates) {
				delegate(widget);
			}
		}

		/// Add one item into the CheckBox with specific delegate.
		void addItem(const std::wstring& title, const OnClickedDelegate& delegate) {
			// TODO Yeah well this all is still hacky so make it better later cengiz :D
			barCursorY += getAABB().getHeight();
			Button* button = new Button(title, getAABB().x1, barCursorY, getAABB().getWidth(), getAABB().getHeight());
			button->bindOnClicked(delegate);
			combBoxItemWidgetList.push_back(button);
		}

		/// Add one item into the CheckBox using the CheckBox delegate.
		void addItem(const std::wstring& title) {
			// TODO Yeah well this all is still hacky so make it better later cengiz :D
			barCursorY += getAABB().getHeight();
			Button* button = new Button(title, getAABB().x1, barCursorY, getAABB().getWidth(), getAABB().getHeight());
			combBoxItemWidgetList.push_back(button);
		}

		std::vector<Widget*>& getWidgets() {
			return combBoxItemWidgetList;
		}

		xdl::xdl_int getCurrentSelectedIndex() const {
			return currentSelectedIndex;
		}


		Widget* getCurrentSelectedItem() const {
			return currentSelectedItem;
		}

		virtual void draw() override;
		
		virtual void setWidgetSceneSystem(XdevLQuadTree<int, Widget*>* wss) {
			widgetSceneSystem = wss;
		}

	public:

		void bindOnItemSelected(const OnItemSelectedDelegateType& delegate) {
			onItemSelectedDelegates.push_back(delegate);
		}

		void unbindOnItemSelected(const OnClickedDelegate& delegate) {
			auto it = std::find(onItemSelectedDelegates.begin(), onItemSelectedDelegates.end(), delegate);
			if(it != onItemSelectedDelegates.end()) {
				onItemSelectedDelegates.erase(it);
			}
		}

	private:
		std::vector<Widget*> combBoxItemWidgetList;
		xdl::xdl_int barCursorY;
		xdl::xdl_bool isActivated;
		xdl::xdl_int currentSelectedIndex;
		Widget* currentSelectedItem;
		Widget::OnClickedDelegate selectedDelegate;
		std::vector<OnItemSelectedDelegateType> onItemSelectedDelegates;
};



void ComboBox::draw() {
	const AABB& aabb = getAABB();
	{
		glBegin(GL_TRIANGLE_STRIP);

		const soan::Color& color = getColor();
		glColor4f(color.r, color.g, color.b, color.a);

		glVertex2i(aabb.x1, aabb.y1);
		glVertex2i(aabb.x1, aabb.y2);
		glVertex2i(aabb.x2, aabb.y1);
		glVertex2i(aabb.x2, aabb.y2);

		glEnd();

		const soan::Color arrowColor(0.3, 0.3, 0.3, 1.0);
		glBegin(GL_TRIANGLE_STRIP);
		xdl::xdl_int x_off = aabb.x1 + aabb.getWidth() - (100.0/(xdl::xdl_float)aabb.getWidth())*20.0f;
		glColor4f(arrowColor.r, arrowColor.g, arrowColor.b, arrowColor.a);
		glVertex2i(x_off, aabb.y1);
		glVertex2i(x_off, aabb.y2);
		glVertex2i(aabb.x2, aabb.y1);
		glVertex2i(aabb.x2, aabb.y2);
		glEnd();

	}

	if(isActivated) {
		if(combBoxItemWidgetList.size() > 0) {

			for(auto& item : combBoxItemWidgetList) {
				glBegin(GL_TRIANGLE_STRIP);
				const soan::Color& color = item->getColor();
				const AABB& itemaabb = item->getAABB();
				glColor4f(color.r, color.g, color.b, color.a);

				glVertex2i(itemaabb.x1, itemaabb.y1);
				glVertex2i(itemaabb.x1, itemaabb.y2);
				glVertex2i(itemaabb.x2, itemaabb.y1);
				glVertex2i(itemaabb.x2, itemaabb.y2);
				glEnd();
			}

		}
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


class MenuBar : public Widget {
	public:
		MenuBar(xdl::xdl_int x, xdl::xdl_int y, xdl::xdl_int width, xdl::xdl_int height) :
			Widget(x, y, width, height),
			barCursorX(x),
			barCursorY(y) {}

		virtual void draw() override;

		ContextMenu* addMenu(const std::wstring& title, xdl::xdl_int width = 80) {
			ContextMenu* contextMenu = new ContextMenu(barCursorX, barCursorY, width, getAABB().getHeight());
			barWidgets.push_back(contextMenu);
			barCursorX+= width;
			
			return contextMenu;
		}

		virtual void onButtonPress(const xdl::XdevLButtonId& buttonid, xdl::xdl_int x, xdl::xdl_int y) override {
			Widget::onButtonPress(buttonid, x, y);

			if(isButtonPressed()) {

			}
		}

		std::vector<ContextMenu*>& getWidgets() {
			return barWidgets;
		}

		/// This is used by the widget scenen system.
		virtual void setWidgetSceneSystem(XdevLQuadTree<int, Widget*>* wss) {
			widgetSceneSystem = wss;
			
			for(auto contextMenu : barWidgets) {
				contextMenu->setWidgetSceneSystem(wss);
				widgetSceneSystem->insertObject(contextMenu);
			}
			
		}


	private:
		std::vector<ContextMenu*> barWidgets;
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
