#ifndef WIDGET_H
#define WIDGET_H

#include "Engine/Color.h"

#include "Engine/GUI/AABB.h"
#include "Engine/GUI/WidgetSceneSystemUtils.h"

/**
 * @class Widget
 * @brief A Widget which is a base class for most widget types.
 */
class Widget {
public:
	typedef xdl::XdevLDelegate<void, const xdl::XdevLButtonId&, const Widget&> WidgetButtonPressedDelegate;
	typedef xdl::XdevLDelegate<void, const xdl::XdevLButtonId&, const Widget&> WidgetButtonReleasedDelegate;

	typedef xdl::XdevLDelegate<void, xdl::xdl_int, xdl::xdl_int, Widget*> OnPointerMotionDelegate;
	typedef xdl::XdevLDelegate<void, Widget*> OnClickedDelegate;
	typedef xdl::XdevLDelegate<void, Widget*> OnPointerHoverDelegate;

	typedef xdl::XdevLDelegate<void, std::list<Widget*>&> ActivateWidgetsDelegateType;
	typedef xdl::XdevLDelegate<void, std::list<Widget*>&> DeactivateWidgetsDelegateType;

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

		// Call delegates if there a one.
//			for(auto delegate : pointerMotionDelegates) {
//				delegate(x, y, this);
//			}

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
	
	void setActiveWidgetListDelegate(ActivateWidgetsDelegateType& activateWidgetsDelegate) {
		activateWidgets = activateWidgetsDelegate;
	}
	
	void setDeactivateWidgetListDelegate(DeactivateWidgetsDelegateType& deactivateWidgetsDelegate) {
		deactivateWidgets = deactivateWidgetsDelegate;
	}

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

//		void bindOnPointerMotionDelegate(const OnPointerMotionDelegate& delegate) {
//			onPointerMotionDelegates.push_back(delegate);
//		}
//
//		void unbindOnPointerMotionDelegate(const OnClickedDelegate& delegate) {
//			auto it = std::find(onPointerMotionDelegates.begin(), onPointerMotionDelegates.end(), delegate);
//			if(it != onPointerMotionDelegates.end()) {
//				onPointerMotionDelegates.erase(it);
//			}
//		}

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
	XdevLQuadTree<int, Widget*>* eventGrid;
	ActivateWidgetsDelegateType activateWidgets;
	DeactivateWidgetsDelegateType deactivateWidgets;
private:
	std::vector<OnClickedDelegate> onClickedDelegates;
	std::vector<OnPointerHoverDelegate> onPointerHoverDelegates;
	std::vector<OnPointerMotionDelegate> onPointerMotionDelegates;

};


#endif
