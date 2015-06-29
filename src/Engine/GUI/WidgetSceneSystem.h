#ifndef WIDGET_SCENE_SYSTEM_H
#define WIDGET_SCENE_SYSTEM_H

#include "Engine/GUI/MenuBar.h"

class WidgetPointerMotionType {
public:
	explicit WidgetPointerMotionType(xdl::XdevLAxisId _id, xdl::xdl_float _value) :
		id(_id),
		value(_value)
	{}
	xdl::XdevLAxisId id;
	xdl::xdl_float value;
};

class WidgetButtonStateType {
public:
	explicit WidgetButtonStateType(xdl::XdevLButtonId _id, xdl::XdevLButtonState _state) :
		id(_id),
		state(_state)
		{
		}
	xdl::XdevLButtonId id;
	xdl::XdevLButtonState state;
};

class WidgetSceneSystem {
public:
		typedef XdevLQuadTree<int, Widget*> QuadTreeType;

		WidgetSceneSystem() :
			m_xaxis(0.0f),
			m_yaxis(0.0f) 
		{
			activateWidgetsDelegate = Widget::ActivateWidgetsDelegateType::Create<WidgetSceneSystem, &WidgetSceneSystem::activateWidgets>(this);
			deactivateWidgetDelegate = Widget::DeactivateWidgetsDelegateType::Create<WidgetSceneSystem, &WidgetSceneSystem::deactivateWidgets>(this);
		}
		
		~WidgetSceneSystem() {

		}
		
		/// Initialize the GUI system.
		xdl::xdl_int init(xdl::xdl_int width, xdl::xdl_int height) {
			eventGrid = new QuadTreeType(0, 0, width , height, 5);
			eventGrid->init();
			return xdl::ERR_OK;
		}
		
		/// Shut down the GUI system.
		void shutdown() {
			eventGrid->shutdown();
			delete eventGrid;
		}
		
		// Draw all widgets.
		// TODO For later improvements only needed parts of the event grid should be drawn. For now just do everything.
		void draw() {
			for(auto& widget : widgets) {
				widget->draw();
			}
		}
		
		/// The current used grid.
		void drawGrid() {
			eventGrid->drawGrid();
		}
		
		/// Draw just one node.
		void drawNode(QuadTreeType::NodeType* node) {
			eventGrid->drawNode(node);
		}

		/// Register one widget.
		void registerWidget(Widget* widget) {
			// TODO What about if the user specifies 2 times the same widget?
			
			widget->setActiveWidgetListDelegate(activateWidgetsDelegate);
			widget->setDeactivateWidgetListDelegate(deactivateWidgetDelegate);
			
			// Add to the event grid.
			eventGrid->insertObject(widget);

			widgets.push_back(widget);
		}
		
		void registerAll(const std::list<Widget*>& widgets) {
			
		}
		
		void unregisterWidget(Widget* widget) {
			auto it = std::find(widgets.begin(), widgets.end(), widget);
			if(it != widgets.end()) {
				widgets.erase(it);
			}
		}
		
		QuadTreeType::NodeType* find(xdl::xdl_int x, xdl::xdl_int y) {
			return eventGrid->find(x, y);
		}
		
		void activateWidgets(std::list<Widget*>& widgets) {
			for(auto& widget : widgets) {
				activeWidgets.push_back(widget);
			}
		}
		
		void deactivateWidgets(std::list<Widget*>& widgets) {
			for(auto& widget : widgets) {
				std::list<Widget*>::iterator it = std::find(activeWidgets.begin(), activeWidgets.end(), widget);
				if(it != activeWidgets.end()) {
					activeWidgets.erase(it);
				}
			}
		}
		
		void onButton(const xdl::XdevLButtonId& id, const xdl::XdevLButtonState& state) {
			buttonStates.push_back(WidgetButtonStateType(id, state));

		}
		
		void onPointerMotion(const xdl::XdevLAxisId& id, const xdl::xdl_float& value) {
			axisStates.push_back(WidgetPointerMotionType(id, value));
		}
		
		std::list<Widget*>& getActiveWidgetList() {
			return activeWidgets;
		}
		
		void update() {

			WidgetSceneSystem::QuadTreeType::NodeType* currentPointerNode = find(m_xaxis, m_yaxis);
			
			//
			// Handle pointer motion events
			// 
			for(auto& axis : axisStates) {
			
				switch(axis.id) {
					case xdl::AXIS_X: {
						m_xaxis = axis.value;
					}
					break;
					case xdl::AXIS_Y: {
						m_yaxis = axis.value;
					}
					break;
					default:
						break;
				}

				WidgetSceneSystem::QuadTreeType::NodeType* currentPointerNode = find(m_xaxis, m_yaxis);
						const  WidgetSceneSystem::QuadTreeType::NodeType::NodeItemVectorType& listOfWidgets = currentPointerNode->getItems();
				for(auto& i : listOfWidgets) {
					i->onMouseMove(m_xaxis, m_yaxis);
				}
				for(auto& i : activeWidgets) {
					i->onMouseMove(m_xaxis, m_yaxis);
				}
			}
			axisStates.clear();
			
			//
			// Handle button events
			// 
			for(auto& button : buttonStates) {
				const WidgetSceneSystem::QuadTreeType::NodeType::NodeItemVectorType& listOfWidgets = currentPointerNode->getItems();
				std::cout << activeWidgets.size() << std::endl;
				if(button.state == xdl::BUTTON_PRESSED) {
					for(auto& i : listOfWidgets) {
						i->onButtonPress(button.id, m_xaxis, m_yaxis);
					}
					for(auto& i : activeWidgets) {
						i->onButtonPress(button.id, m_xaxis, m_yaxis);
					}
				} else
				if(button.state == xdl::BUTTON_RELEASED) {
					for(auto& i : listOfWidgets) {
						i->onButtonRelease(button.id, m_xaxis, m_yaxis);
					}
					for(auto& i : activeWidgets) {
						i->onButtonRelease(button.id, m_xaxis, m_yaxis);
					}
				}
			}
			buttonStates.clear();
		}
		
private:
		std::list<WidgetPointerMotionType> axisStates;
		std::list<WidgetButtonStateType> buttonStates;
		std::list<Widget*> activeWidgets;
		std::list<Widget*> widgets;
		QuadTreeType* eventGrid;
		Widget::ActivateWidgetsDelegateType activateWidgetsDelegate;
		Widget::DeactivateWidgetsDelegateType deactivateWidgetDelegate;
		
		xdl::xdl_float 					m_xaxis;
		xdl::xdl_float					m_yaxis;
};


#endif
