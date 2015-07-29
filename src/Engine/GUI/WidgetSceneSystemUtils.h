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

#ifndef WIDGET_SCENE_SYSTEM_UTILS_H
#define WIDGET_SCENE_SYSTEM_UTILS_H

#include <Engine/GUI/Node.h>

template<typename T, typename OBJ>
class XdevLQuadTree {
	public:
		typedef XdevLQuadTreeNode<T, OBJ> NodeType;
		typedef xdl::XdevLDelegate<void, NodeType*> DrawNodeDelegateType;

		XdevLQuadTree(xdl::xdl_int x, xdl::xdl_int y, xdl::xdl_uint width, xdl::xdl_uint height, xdl::xdl_uint depth) :
			aabb(x, y, x+width, y+height),
			root(nullptr),
			depth(depth) {

		}

		~XdevLQuadTree() {
			assert( (root == nullptr) && "XdevLQuadtTree:init: Quadtree wasn't shutdown before. Please shutdown before deleting this object.");
		}

		void init() {
			assert( (root == nullptr) && "XdevLQuadtTree:init: Quadtree wasn't shutdown before. Please shutdown before calling this method again.");
			root = new NodeType(aabb);
			//create_recursive(0, root);
		}

		void shutdown() {
			delete_recursive(0, root);
			root = nullptr;
		}

		xdl::xdl_uint getWidth() const {
			return aabb.getWidth();
		}

		xdl::xdl_uint getHeight() const {
			return aabb.getHeight();
		}

		void setDrawNodeCallbackType(DrawNodeDelegateType callback) {
			drawNodeCallback = callback;
		}

		void insertObject(OBJ obj) {
			NodeType* node = new NodeType();
			obj->setWidgetSceneSystem(this);
			node->setAABB(obj->getAABB());
			node->addItem(obj);
			insert(node);
			delete node;
		}

		void insertObjectAll(std::vector<OBJ>& objs) {

			for(auto& obj : objs) {
				insertObject(obj);
			}
		}


		NodeType* insert(NodeType* item) {
			return insert_recursive(0, root, item);
		}

		NodeType* insert_recursive(xdl::xdl_uint level, NodeType* node, NodeType* item) {

			// Check if the item fits into this node?
			if((node->aabb.x1 <= item->aabb.x1) &&
			        (node->aabb.x2 >= item->aabb.x2) &&
			        (node->aabb.y1 <= item->aabb.y1) &&
			        (node->aabb.y2 >= item->aabb.y2)) {
				node->anyItems(true);
			}

			// We reached the maximum allowed depth which is the leaf node, so add it definitely.
			if(level == depth) {
				node->addItem(item->getFirstItem());
				node->anyItems(true);
				return node;
			}


			T mid_h = node->aabb.y1 + (node->aabb.y2 - node->aabb.y1)/2;
			T mid_v = node->aabb.x1 + (node->aabb.x2 - node->aabb.x1)/2;

			bool upper_quad	= (item->aabb.y1 < mid_h) && (item->aabb.y2 < mid_h);
			bool lower_quad	= (item->aabb.y1 > mid_h);
			bool left_quad 	= (item->aabb.x1 < mid_v) && (item->aabb.x2 < mid_v);
			bool right_quad	= (item->aabb.x1 > mid_v);

			if(upper_quad) {
				if(left_quad) {
					if(!node->hasChildren()) {
						node->split();
					}
					insert_recursive(level + 1, node->top_left, item);
				} else if(right_quad) {
					if(!node->hasChildren()) {
						node->split();
					}
					insert_recursive(level + 1, node->top_right, item);
				} else {
					if(!node->hasChildren()) {
						node->split();
					}
					insert_recursive(level + 1, node->top_left, item);
					insert_recursive(level + 1, node->top_right, item);
				}
			} else if(lower_quad) {
				if(left_quad) {
					if(!node->hasChildren()) {
						node->split();
					}
					insert_recursive(level + 1, node->bottom_left, item);
				} else if(right_quad) {
					if(!node->hasChildren()) {
						node->split();
					}
					insert_recursive(level + 1, node->bottom_right, item);
				} else {
					if(!node->hasChildren()) {
						node->split();
					}
					insert_recursive(level + 1, node->bottom_left, item);
					insert_recursive(level + 1, node->bottom_right, item);
				}

			} else {
				if(left_quad) {
					if(!node->hasChildren()) {
						node->split();
					}
					insert_recursive(level + 1, node->top_left, item);
					insert_recursive(level + 1, node->bottom_left, item);
				} else if(right_quad) {
					if(!node->hasChildren()) {
						node->split();
					}
					insert_recursive(level + 1, node->top_right, item);
					insert_recursive(level + 1, node->bottom_right, item);
				} else {
					if(!node->hasChildren()) {
						node->split();
					}
					insert_recursive(level + 1, node->top_left, item);
					insert_recursive(level + 1, node->bottom_left, item);
					insert_recursive(level + 1, node->top_right, item);
					insert_recursive(level + 1, node->bottom_right, item);
				}
			}
			node->anyItems(true);
			return node;
		}

		void removeObjectAll(std::vector<OBJ>& objs) {
			for(auto& obj : objs) {
				removeObject(obj);
			}
		}

		void removeObject(OBJ obj) {
			remove_recursive(0, root, obj);
		}

		void remove_recursive(xdl::xdl_int level, NodeType* node, OBJ obj) {
			// TODO How about deleting the node if the size gets 0?

			if(level != depth) {
				if(node->top_left && node->top_left->hasItems) {
					remove_recursive(level + 1, node->top_left, obj);
				}
				if(node->top_right && node->top_right->hasItems) {
					remove_recursive(level + 1, node->top_right, obj);
				}
				if(node->bottom_left && node->bottom_left->hasItems) {
					remove_recursive(level + 1, node->bottom_left, obj);
				}
				if(node->bottom_right && node->bottom_right->hasItems) {
					remove_recursive(level + 1, node->bottom_right, obj);
				}
				return;
			}

			auto it = std::find(node->getItems().begin(), node->getItems().end(), obj);
			if(it != node->getItems().end()) {
				node->getItems().erase(it);
			}
		}


		NodeType* find(xdl::xdl_int x, xdl::xdl_int y) {
			return find_recursive(0, root, x, y);
		}

		NodeType* find_recursive(xdl::xdl_int level, NodeType* node, xdl::xdl_int x, xdl::xdl_int y) {

			// Do not search in a quadrant if there is nothing or if we reached the leaf node.
			if((level == depth) || !node->anyItems()) {
				return node;
			}

			bool upper_quad = (y < node->aabb.y1 + (node->aabb.y2 - node->aabb.y1)/2);
			bool left_quad = (x < (node->aabb.x1 + (node->aabb.x2 - node->aabb.x1)/2));

			if(upper_quad) {
				// Pointer is in the upper part.
				if(left_quad) {
					if(node->top_left == nullptr) {
						return node;
					} else {
						return find_recursive(level + 1, node->top_left, x, y);
					}
				} else {
					if(node->top_right == nullptr) {
						return node;
					} else {
						return find_recursive(level + 1,node->top_right, x, y);
					}
				}

			} else {
				// Pointer is in bottom part.
				if(left_quad) {
					if(node->bottom_left == nullptr) {
						return node;
					} else {
						return find_recursive(level + 1,node->bottom_left, x, y);
					}
				} else {
					if(node->bottom_right == nullptr) {
						return node;
					} else {
						return find_recursive(level + 1,node->bottom_right, x, y);
					}
				}
			}
			return node;
		}

		void drawGrid() {
			draw_recursive(0, root);
		}


		void create_recursive(xdl::xdl_uint level, NodeType* node) {

			node->split();

			if(level == depth) {
				return;
			}

			create_recursive(level + 1, node->top_left);
			create_recursive(level + 1, node->top_right);
			create_recursive(level + 1, node->bottom_left);
			create_recursive(level + 1, node->bottom_right);
		}

		void delete_recursive(xdl::xdl_int level, NodeType* node) {

			if(level == depth) {
				return;
			}

			if(node->top_left) {
				delete_recursive(level + 1, node->top_left);
			}
			if(node->top_right) {
				delete_recursive(level + 1, node->top_right);
			}
			if(node->bottom_left) {
				delete_recursive(level + 1, node->bottom_left);
			}
			if(node->bottom_right) {
				delete_recursive(level + 1, node->bottom_right);
			}

			delete(node);
		}

		void draw_recursive(int level, NodeType* node) {
			if(node == nullptr) {
				return;
			}

			drawNode(node);

			if(level == depth) {
				return;
			}

			if(node->top_left) {
				draw_recursive(level + 1, node->top_left);
			}
			if(node->top_right) {
				draw_recursive(level + 1, node->top_right);
			}
			if(node->bottom_left) {
				draw_recursive(level + 1, node->bottom_left);
			}
			if(node->bottom_right) {
				draw_recursive(level + 1, node->bottom_right);
			}
		}


		void drawNode(NodeType* node) {
			if(node == nullptr) {
				return;
			}

			if(drawNodeCallback.isValid()) {
				drawNodeCallback(node);
			}
		}

	private:
		AABB aabb;
		NodeType* root;
		xdl::xdl_uint depth;
		DrawNodeDelegateType drawNodeCallback;
};


#endif
