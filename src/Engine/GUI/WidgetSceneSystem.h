#ifndef WIDGET_SCENE_SYSTEM_H
#define WIDGET_SCENE_SYSTEM_H

#include "Engine/GUI/Widgets.h"

class WidgetSceneSystem {
public:
		typedef XdevLQuadTree<int, Widget*> QuadTreeType;
		
		WidgetSceneSystem() {
			
		}
		~WidgetSceneSystem() {

		}
		
		xdl::xdl_int init(xdl::xdl_int width, xdl::xdl_int height) {
			eventGrid = new QuadTreeType(0, 0, width , height, 5);
			eventGrid->init();
			return xdl::ERR_OK;
		}
		
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
		
		void drawGrid() {
			eventGrid->drawGrid();
		}
		
		void drawNode(QuadTreeType::NodeType* node) {
			eventGrid->drawNode(node);
		}

		void resgister(Widget* widget) {
			// TODO What about if the user specifies 2 times the same widget?
			
			// Add to the event grid.
			eventGrid->insertObject(widget);

			widgets.push_back(widget);
		}
		
		void registerAll(const std::list<Widget*>& widgets) {
			//eventGrid->in
		}
		
		void unregister(Widget* widget) {
			auto it = std::find(widgets.begin(), widgets.end(), widget);
			if(it != widgets.end()) {
				widgets.erase(it);
			}
		}
		
		QuadTreeType::NodeType* find(xdl::xdl_int x, xdl::xdl_int y) {
			return eventGrid->find(x, y);
		}
		
	private:
		std::vector<Widget*> widgets;
		QuadTreeType* eventGrid;
};


#endif