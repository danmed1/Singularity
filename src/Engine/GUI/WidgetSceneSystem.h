#ifndef WIDGET_SCENE_SYSTEM_H
#define WIDGET_SCENE_SYSTEM_H

#include "Engine/GUI/MenuBar.h"

class WidgetSceneSystem {
public:
		typedef XdevLQuadTree<int, Widget*> QuadTreeType;

		WidgetSceneSystem() :
			m_xaxis(0.0f),
			m_yaxis(0.0f),
			currentPointerNode(nullptr)
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

			const WidgetSceneSystem::QuadTreeType::NodeType::NodeItemVectorType& listOfWidgets = currentPointerNode->getItems();
			std::cout << activeWidgets.size() << std::endl;
			if(state == xdl::BUTTON_PRESSED) {
				for(auto& i : listOfWidgets) {
					i->onButtonPress(id, m_xaxis, m_yaxis);
				}
				for(auto& i : activeWidgets) {
					i->onButtonPress(id, m_xaxis, m_yaxis);
				}
			} else
			if(state == xdl::BUTTON_RELEASED) {
				for(auto& i : listOfWidgets) {
					i->onButtonRelease(id, m_xaxis, m_yaxis);
				}
				for(auto& i : activeWidgets) {
					i->onButtonRelease(id, m_xaxis, m_yaxis);
				}
			}
		}
		
		void onPointerMotion(const xdl::XdevLAxisId& id, const xdl::xdl_float& value) {
			switch(id) {
				case xdl::AXIS_X: {
					m_xaxis = value;
				}
				break;
				case xdl::AXIS_Y: {
					m_yaxis = value;
				}
				break;
				default:
					break;
			}
			currentPointerNode = find(m_xaxis, m_yaxis);
		//	std::cout << m_currentPointerNode->getNumberOfItems() << std::endl;
			const  WidgetSceneSystem::QuadTreeType::NodeType::NodeItemVectorType& listOfWidgets = currentPointerNode->getItems();
			for(auto& i : listOfWidgets) {
				i->onMouseMove(m_xaxis, m_yaxis);
			}
			for(auto& i : activeWidgets) {
				i->onMouseMove(m_xaxis, m_yaxis);
			}
		}
		
		std::list<Widget*>& getActiveWidgetList() {
			return activeWidgets;
		}
		
		QuadTreeType::NodeType* getCurrentPointerNode() const {
			return currentPointerNode;
		}
		
private:
		std::list<Widget*> activeWidgets;
		std::list<Widget*> widgets;
		QuadTreeType* eventGrid;
		Widget::ActivateWidgetsDelegateType activateWidgetsDelegate;
		Widget::DeactivateWidgetsDelegateType deactivateWidgetDelegate;
		
		xdl::xdl_float 					m_xaxis;
		xdl::xdl_float					m_yaxis;
		QuadTreeType::NodeType* currentPointerNode;
};


#endif
