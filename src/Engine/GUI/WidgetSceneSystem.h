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

#ifndef WIDGET_SCENE_SYSTEM_H
#define WIDGET_SCENE_SYSTEM_H

#include <XdevLWindow/XdevLWindow.h>

#include "Engine/GUI/MenuBar.h"

class WidgetSceneSystem {
	public:
		typedef XdevLQuadTree<int, Widget*> QuadTreeType;

		WidgetSceneSystem(xdl::IPXdevLCore core) :
			eventGrid(nullptr),
			m_xaxis(0.0f),
			m_yaxis(0.0f),
			currentPointerNode(nullptr),
			m_canvas(nullptr) {
			spawnPopupWindowDelegate  = Widget::SpawnPopupWindowDelegateType::Create<WidgetSceneSystem, &WidgetSceneSystem::spawnPopupWindow>(this);
			destroyWindowDelegate = Widget::DestroyWindowDelegateType::Create<WidgetSceneSystem, &WidgetSceneSystem::destroyWindow>(this);
			activateWidgetsDelegate = Widget::ActivateWidgetsDelegateType::Create<WidgetSceneSystem, &WidgetSceneSystem::activateWidgets>(this);
			deactivateWidgetDelegate = Widget::DeactivateWidgetsDelegateType::Create<WidgetSceneSystem, &WidgetSceneSystem::deactivateWidgets>(this);

			windowServer = xdl::createModule<xdl::IPXdevLWindowServer>(core, xdl::XdevLModuleName("XdevLWindowServer"), xdl::XdevLID("MyWindowServer"));
		}

		~WidgetSceneSystem() {

		}

		/// Initialize the GUI system.
		xdl::xdl_int init(xdl::xdl_int width, xdl::xdl_int height) {
			if(eventGrid != nullptr) {
				eventGrid->shutdown();
				delete eventGrid;
			}

			eventGrid = new QuadTreeType(0, 0, width , height, 2);
			eventGrid->init();

			if(widgets.size() > 0) {
				for(auto& widget : widgets) {
					eventGrid->insertObject(widget);
				}
			}

			return xdl::ERR_OK;
		}


		/// Shut down the GUI system.
		void shutdown() {
			eventGrid->shutdown();
			delete eventGrid;
			eventGrid = nullptr;
		}

		void setCanvas(soan::Canvas* canvas) {
			m_canvas = canvas;
		}

		void setDrawNodeCallbackType(QuadTreeType::DrawNodeDelegateType callback) {
			eventGrid->setDrawNodeCallbackType(callback);
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
			m_canvas->setCurrentColor(soan::Color(255, 255, 255, 255));
			eventGrid->drawGrid();
		}

		/// Draw just one node.
		void drawNode(QuadTreeType::NodeType* node) {
			m_canvas->setCurrentColor(soan::Color(255, 0, 0, 255));
			eventGrid->drawNode(node);
		}

		/// Register one widget.
		void registerWidget(Widget* widget) {
			// TODO What about if the user specifies 2 times the same widget?

			widget->setActiveWidgetListDelegate(activateWidgetsDelegate);
			widget->setDeactivateWidgetListDelegate(deactivateWidgetDelegate);
			widget->setSpawnPopupWindowDelegate(spawnPopupWindowDelegate);
			widget->setDestroyWindowDelegate(destroyWindowDelegate);

			// Add to the event grid.
			eventGrid->insertObject(widget);

			widgets.push_back(widget);
		}

		void registerAll(const std::list<Widget*>& widgetList) {
			for(auto& widget : widgetList) {
				eventGrid->insertObject(widget);
				widgets.push_back(widget);
			}
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

		void onFocusLost(xdl::xdl_uint windowID) {
//			if(activeWidgets.size() > 0) {
//				deactivateWidgets(activeWidgets);
//			}
		}

		void onButton(const xdl::XdevLButtonId& id, const xdl::XdevLButtonState& state) {

			const WidgetSceneSystem::QuadTreeType::NodeType::NodeItemVectorType& listOfWidgets = currentPointerNode->getItems();
			if(state == xdl::BUTTON_PRESSED) {
				for(auto& i : listOfWidgets) {
					i->onButtonPress(id, m_xaxis, m_yaxis);
				}
				for(auto& i : activeWidgets) {
					i->onButtonPress(id, m_xaxis, m_yaxis);
				}
			} else if(state == xdl::BUTTON_RELEASED) {
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

		void spawnPopupWindow(xdl::XdevLWindow** window, const xdl::XdevLWindowTitle& title, const xdl::XdevLWindowSize& size, const xdl::XdevLWindowPosition& pos) {
			windowServer->createWindow(window, title, pos, size);
		}
		
		void destroyWindow(xdl::XdevLWindow* window) {
			windowServer->destroy(window);
		}

	private:
		xdl::IPXdevLWindowServer windowServer;

		std::list<Widget*> activeWidgets;
		std::list<Widget*> widgets;
		QuadTreeType* eventGrid;
		Widget::SpawnPopupWindowDelegateType spawnPopupWindowDelegate;
		Widget::DestroyWindowDelegateType destroyWindowDelegate;
		Widget::ActivateWidgetsDelegateType activateWidgetsDelegate;
		Widget::DeactivateWidgetsDelegateType deactivateWidgetDelegate;

		xdl::xdl_float 					m_xaxis;
		xdl::xdl_float					m_yaxis;
		QuadTreeType::NodeType* currentPointerNode;
		soan::Canvas*					m_canvas;
};


#endif
