#ifndef WIDGET_SCENE_SYSTEM_UTILS_H
#define WIDGET_SCENE_SYSTEM_UTILS_H

template<typename T, typename OBJ>
class XdevLQuadTree;

/**
 * @class AABB
 * @brief Axis Aligned Bounding Box
 */
class AABB {
public:
	AABB() : x1(0), y1(0), x2(0), y2(0) {}

	AABB(xdl::xdl_int _x1, xdl::xdl_int _y1, xdl::xdl_int _x2, xdl::xdl_int _y2) : x1(_x1), y1(_y1), x2(_x2), y2(_y2) {}

	xdl::xdl_bool isPointInside(xdl::xdl_int x, xdl::xdl_int y) {
		return ((x1 <= x) && (x <= x2) && (y1 <= y) && (y <= y2));
	}
	xdl::xdl_int getWidth() const {
		return (x2-x1);
	}
	xdl::xdl_int getHeight() const {
		return (y2-y1);
	}
	xdl::xdl_int x1, y1, x2, y2;
};


template<typename T, typename OBJ>
struct XdevLQuadTreeNode {
	friend class XdevLQuadTree<T, OBJ>;
	typedef std::list<OBJ> NodeItemVectorType;

	XdevLQuadTreeNode() :
		top_left(nullptr),
		top_right(nullptr),
		bottom_left(nullptr),
		bottom_right(nullptr),
		hasItems(xdl::xdl_false) {}

	XdevLQuadTreeNode(const AABB& _aabb) : 
		aabb(_aabb),
		top_left(nullptr),
		top_right(nullptr),
		bottom_left(nullptr),
		bottom_right(nullptr),
		hasItems(xdl::xdl_false)
	{}

	/// Returns the stored user item of this node.
	const OBJ getFirstItem() const {
		return *items.begin();
	}

	NodeItemVectorType& getItems() {
		return items;
	}

	xdl::xdl_uint getNumberOfItems() {
		return items.size();
	}

	/// Sets the user item for this node.
	void addItem(const OBJ& itm) {
		items.push_back(itm);
	}

	const AABB& getAABB() const {
		return aabb;
	}

	void setAABB(const AABB& _aabb) {
		aabb = _aabb;
	}

	const bool& anyItems() const {
		return hasItems;
	}

	void anyItems(bool state) {
		hasItems = state;
	}

	void split() {
		top_left		= new XdevLQuadTreeNode<T, OBJ>(AABB(aabb.x1, aabb.y1, aabb.x1 + (aabb.x2-aabb.x1)/2, aabb.y1 +(aabb.y2-aabb.y1)/2));
		top_right		= new XdevLQuadTreeNode<T, OBJ>(AABB(aabb.x1 + (aabb.x2-aabb.x1)/2 , aabb.y1, aabb.x2, aabb.y1 + (aabb.y2-aabb.y1)/2));
		bottom_left		= new XdevLQuadTreeNode<T, OBJ>(AABB(aabb.x1, aabb.y1 + (aabb.y2-aabb.y1)/2, aabb.x1 + (aabb.x2-aabb.x1)/2, aabb.y2));
		bottom_right	= new XdevLQuadTreeNode<T, OBJ>(AABB(aabb.x1 + (aabb.x2-aabb.x1)/2, aabb.y1 + (aabb.y2-aabb.y1)/2, aabb.x2, aabb.y2));
	}

	xdl::xdl_bool hasChildren() {
		return (nullptr != top_left) && (nullptr != top_right) && (nullptr != bottom_left) && (nullptr != bottom_right);
	}
private:
	AABB aabb;

	XdevLQuadTreeNode* top_left;
	XdevLQuadTreeNode* top_right;
	XdevLQuadTreeNode* bottom_left;
	XdevLQuadTreeNode* bottom_right;

	xdl::xdl_bool hasItems;

	// Holds an user defined object.
	NodeItemVectorType items;
};

template<typename T, typename OBJ>
class XdevLQuadTree {
public:
	typedef XdevLQuadTreeNode<T, OBJ> NodeType;

	XdevLQuadTree(xdl::xdl_int x, xdl::xdl_int y, xdl::xdl_uint width, xdl::xdl_uint height, xdl::xdl_uint depth) :
		aabb(x, y, x+width, y+height),
		root(nullptr),
		depth(depth) {

	}

	~XdevLQuadTree() {

	}

	void init() {
		root = new NodeType(aabb);
		//create_recursive(0, root);
	}

	void shutdown() {
		delete_recursive(0, root);
		root = nullptr;
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
		
		glVertex2i(node->aabb.x1, node->aabb.y1);
		glVertex2i(node->aabb.x2, node->aabb.y1);

		glVertex2i(node->aabb.x2, node->aabb.y1);
		glVertex2i(node->aabb.x2, node->aabb.y2);

		glVertex2i(node->aabb.x2, node->aabb.y2);
		glVertex2i(node->aabb.x1, node->aabb.y2);

		glVertex2i(node->aabb.x1, node->aabb.y2);
		glVertex2i(node->aabb.x1, node->aabb.y1);
	}

private:
	AABB aabb;
	NodeType* root;
	xdl::xdl_uint depth;
};


#endif
