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

#ifndef COMBO_BOX_H
#define COMBO_BOX_H

#include "Engine/GUI/Button.h"
#include <map>
/**
	@class ComboBox
	@brief A combobox class
	
	A ComboBox open a popup on activation and provides a list of items that can be selected.
	Those items are internally represented by a Button. There are two different ways to get
	informed if an item got selected.
	- Items are using as a base class the Button class. Assign a OnClicked delegate to each item you add.
	- Use the OnItemSelectedDelegate. The assigned delegate will be used to call the callee when an item
	got selected.
*/
class ComboBox : public Widget {
public:
	typedef std::list<Widget*> ItemListType;
	typedef xdl::XdevLDelegate<void, xdl::xdl_uint, Widget*> OnItemSelectedDelegateType;

	ComboBox(xdl::xdl_int x, xdl::xdl_int y, xdl::xdl_int width, xdl::xdl_int height) :
		Widget(x, y, width, height),
		barCursorY(y),
		isActivated(xdl::xdl_false),
		currentSelectedIndex(0),
		currentSelectedItem(nullptr),
		deactivateWidgetsFlag(xdl::xdl_false) {

		// Change color when mouse hovers.
		setHighLightOnMouseHover(xdl::xdl_true);

		// Change color when mouse button is pressed.
		setChangeColorOnButtonPress(xdl::xdl_true);

		// Create a default delegate that handles user selections. We have to know which item will be clicked by the user. 
		// We use this delegate to tell the items (which are derived from the Button class) in the combo box list that we are i
		// nterested if they get clicked.
		selectedDelegate = Widget::OnClickedDelegate::Create<ComboBox, &ComboBox::onSelectedClicked>(this);
	}
	
	~ComboBox() {
		
		// Delete everything which is in the item list.
		for(auto& item : combBoxItemWidgetList) {
			delete item;
		}
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
				activateWidgets(combBoxItemWidgetList);
				
			} else {

				// No that means the ComboBox got deactivated. First we remove all delegates that handles
				// selection events.
				for(auto& widget : combBoxItemWidgetList) {
					widget->unbindOnClicked(selectedDelegate);
				}

				// And then we deactivate all widgets from the event grid.
				deactivateWidgets(combBoxItemWidgetList);
			}
		}
	}

	virtual void onButtonRelease(const xdl::XdevLButtonId& buttonid, xdl::xdl_int x, xdl::xdl_int y) override {
		Widget::onButtonRelease(buttonid, x, y);
		
		// Deactivate the active popup the CheckBox is opening.
		if(deactivateWidgetsFlag) {
			deactivateWidgets(combBoxItemWidgetList);
			deactivateWidgetsFlag = xdl::xdl_false;
		}
	}

	void onSelectedClicked(Widget* widget) {
		// If this method is called the user selected an item in the ComboBox list. We have to deactivate the ComboBox
		// remove all widgets from the event grid and unbind the delegates to handle selection events from the user.

		for(auto& delegate : onItemSelectedDelegates) {
			// TODO Better use not a pointer as a key for the map?
			delegate(comboBoxItemWidgetMap[widget], widget);
		}

		isActivated = xdl::xdl_false;
		currentSelectedItem = widget;

		for(auto& widget : combBoxItemWidgetList) {
			widget->unbindOnClicked(selectedDelegate);
		}
		
		// Why not deleting the widgets list here? The WidgetSystem will inform the widgets about
		// events. It will use a list of widgets. Because it will use a for loop of widgets 
		// it will expect all widgets in the list but because this ComboBox widget would remove the
		// widgets from the active widget list. So the for loop in the WidgetSystem would get corrupt.
		// What we do is, we deffer the delete event to the Button release event.
		deactivateWidgetsFlag = xdl::xdl_true;

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
	void addItem(const std::wstring& title, xdl::xdl_uint id) {
		// TODO Yeah well this all is still hacky so make it better later cengiz :D
		barCursorY += getAABB().getHeight();
		Button* button = new Button(title, getAABB().x1, barCursorY, getAABB().getWidth(), getAABB().getHeight());
		combBoxItemWidgetList.push_back(button);
		comboBoxItemWidgetMap[button] = id;
	}
	
	void removeItem(xdl::xdl_int index) {
		ItemListType::iterator it(combBoxItemWidgetList.begin());
		std::advance(it, index);
		combBoxItemWidgetList.erase(it);
	}

	std::list<Widget*>& getWidgets() {
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

	void unbindOnItemSelected(const OnItemSelectedDelegateType& delegate) {
		onItemSelectedDelegates.remove(delegate);
	}

private:
	// Holds all items in the ComboBox.
	ItemListType combBoxItemWidgetList;
	
	std::map<Widget*, xdl::xdl_uint> comboBoxItemWidgetMap;
	
	// The current cursor position in the activated list.
	xdl::xdl_int barCursorY;
	xdl::xdl_bool isActivated;
	xdl::xdl_int currentSelectedIndex;
	Widget* currentSelectedItem;
	Widget::OnClickedDelegate selectedDelegate;
	std::list<OnItemSelectedDelegateType> onItemSelectedDelegates;
	
	xdl::xdl_bool deactivateWidgetsFlag;
};



void ComboBox::draw() {
	const AABB& aabb = getAABB();
	
	// Draw the activate button.
	{
		glBegin(GL_TRIANGLE_STRIP);

		const soan::Color& color = getColor();
		glColor4f(color.r, color.g, color.b, color.a);

		glVertex2i(aabb.x1, aabb.y1);
		glVertex2i(aabb.x1, aabb.y2);
		glVertex2i(aabb.x2, aabb.y1);
		glVertex2i(aabb.x2, aabb.y2);

		glEnd();

		// Draw the arrow for the activate button.
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

			// Draw the background of the items list.
			xdl::xdl_int height = 0;
			for(auto& item : combBoxItemWidgetList) {
				const AABB& itemaabb = item->getAABB();
				height += itemaabb.getHeight();
			}

			glBegin(GL_TRIANGLE_STRIP);
				const soan::Color& borderColor = getBorderColor();
				glColor4f(borderColor.r, borderColor.g, borderColor.b, borderColor.a);

				glVertex2i(aabb.x1 - getBorderSize(), aabb.y2);
				glVertex2i(aabb.x1 - getBorderSize(), aabb.y2 + height + getBorderSize());
				glVertex2i(aabb.x2 + getBorderSize(), aabb.y2);
				glVertex2i(aabb.x2 + getBorderSize(), aabb.y2 + height + getBorderSize());
			glEnd();
			
			
			
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


#endif
