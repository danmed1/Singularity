#ifndef GUI_WIDGETS_H
#define GUI_WIDGETS_H

#include "Engine/GUI/Widget.h"
#include "Engine/GUI/WidgetSceneSystemUtils.h"

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
					eventGrid->insertObjectAll(combBoxItemWidgetList);
				} else {

					// No that means the ComboBox got deactivated. First we remove all delegates that handles
					// selection events.
					for(auto& widget : combBoxItemWidgetList) {
						widget->unbindOnClicked(selectedDelegate);
					}

					// And then we deactivate all widgets from the event grid.
					eventGrid->removeObjectAll(combBoxItemWidgetList);
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
			eventGrid->removeObjectAll(combBoxItemWidgetList);

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
			eventGrid = wss;
		}

		void deActivate() {
			isActivated = xdl::xdl_false;
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


class MenuBar : public Widget {
	public:
		MenuBar(xdl::xdl_int x, xdl::xdl_int y, xdl::xdl_int width, xdl::xdl_int height) :
			Widget(x, y, width, height),
			barCursorX(x),
			barCursorY(y),
			currentSelectedContextMenu(nullptr)
		{
			onClickDelegate = Widget::OnClickedDelegate::Create<MenuBar, &MenuBar::onSelectedClicked>(this);
			onPointerMotionDelegate = Widget::OnPointerMotionDelegate::Create<MenuBar, &MenuBar::onPointerMotion>(this);
			
		}

		virtual void draw() override;

		ContextMenu* addMenu(const std::wstring& title, xdl::xdl_int width = 80) {
			ContextMenu* contextMenu = new ContextMenu(barCursorX, barCursorY, width, getAABB().getHeight());
			barWidgets.push_back(contextMenu);
			barCursorX+= width;
			contextMenu->bindOnClicked(onClickDelegate);
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
			eventGrid = wss;

			for(auto contextMenu : barWidgets) {
				contextMenu->setWidgetSceneSystem(wss);
				eventGrid->insertObject(contextMenu);
			}

		}
		
		// The user clicked one of the items in the context menu. We handle the stuff here.
		void onSelectedClicked(Widget* widget) {
			ContextMenu* contextMenu = static_cast<ContextMenu*>(widget);
			if( (currentSelectedContextMenu != nullptr) && (currentSelectedContextMenu != contextMenu) ) {
				currentSelectedContextMenu->deActivate();
			}
			currentSelectedContextMenu =contextMenu;
		}
		
		void onPointerMotion(xdl::xdl_int x, xdl::xdl_int y, Widget* widget) {
			std::cout << "Motion: " << x << ", " << y << std::endl;
		}

	private:
		std::vector<ContextMenu*> barWidgets;
		xdl::xdl_int barCursorX;
		xdl::xdl_int barCursorY;
		Widget::OnClickedDelegate onClickDelegate;
		Widget::OnPointerMotionDelegate onPointerMotionDelegate;
		ContextMenu* currentSelectedContextMenu;
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
