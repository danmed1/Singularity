#ifndef WIDGET_SCENE_SYSTEM_H
#define WIDGET_SCENE_SYSTEM_H

#include "Engine/GUI/MenuBar.h"

class WidgetSceneSystem {
public:
		typedef XdevLQuadTree<int, Widget*> QuadTreeType;

		WidgetSceneSystem() {
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
		
		void onButtonHandle(const xdl::XdevLButtonId& id, const xdl::XdevLButtonState& state) {
			
		}
		
		std::list<Widget*>& getActiveWidgetList() {
			return activeWidgets;
		}
		
private:
		std::list<Widget*> activeWidgets;
		std::list<Widget*> widgets;
		QuadTreeType* eventGrid;
		Widget::ActivateWidgetsDelegateType activateWidgetsDelegate;
		Widget::DeactivateWidgetsDelegateType deactivateWidgetDelegate;
};


#endif