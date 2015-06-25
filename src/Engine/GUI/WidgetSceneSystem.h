#ifndef WIDGET_SCENE_SYSTEM_H
#define WIDGET_SCENE_SYSTEM_H

template<typename T, typename OBJ>
class XdevLQuadTree;

template<typename T, typename OBJ>
struct XdevLQuadTreeNode {
		friend class XdevLQuadTree<T, OBJ>;
		typedef std::vector<OBJ> NodeItemVectorType;
		XdevLQuadTreeNode(const AABB& _aabb) : aabb(_aabb) {}

		XdevLQuadTreeNode() :
			top_left(nullptr),
			top_right(nullptr),
			bottom_left(nullptr),
			bottom_right(nullptr),
			hasItems(xdl::xdl_false) {}

		/// Returns the stored user item of this node.
		const OBJ& getItem(xdl::xdl_int idx) const {
			return items[idx];
		}

		const NodeItemVectorType& getItems() const {
			return items;
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


		xdl::xdl_uint getNumberOfAssigedNodes() {
			return assignedNodes.size();
		}

		void addNode(XdevLQuadTreeNode* node) {
			assignedNodes.push_back(node);
		}

	private:

		XdevLQuadTreeNode* top_left;
		XdevLQuadTreeNode* top_right;
		XdevLQuadTreeNode* bottom_left;
		XdevLQuadTreeNode* bottom_right;

		AABB aabb;

		xdl::xdl_bool hasItems;

		std::vector<XdevLQuadTreeNode*> assignedNodes;

		// Holds an user defined object.
		NodeItemVectorType items;
};

template<typename T, typename OBJ>
class XdevLQuadTree {
	public:
		typedef XdevLQuadTreeNode<T, OBJ> NodeType;

		XdevLQuadTree(xdl::xdl_int x, xdl::xdl_int y, xdl::xdl_uint width, xdl::xdl_uint height, xdl::xdl_uint depth) :
			root(new NodeType(AABB(x, y, x + width, y + height))),
			width(width),
			height(height),
			depth(depth) {

		}

		~XdevLQuadTree() {

		}

		void init() {
			create_recursive(0, root);
		}

		void shutdown() {
			delete_recursive(0, root);
			root = nullptr;
		}

		NodeType* insertObject(OBJ obj) {
			NodeType* node = new NodeType();
			node->setAABB(obj->getAABB());
			node->addItem(obj);
			insert(node);
			return node;
		}

		void insertObjectAll(std::vector<OBJ>& objs) {

			for(auto& obj : objs) {
				insertObject(obj);
			}
		}


		void insert(NodeType* item) {
			insert_recursive(0, root, item);
		}

		void insert_recursive(xdl::xdl_uint level, NodeType* root, NodeType* item) {

			// Check if the item fits into this node?
			if((root->aabb.x1 < item->aabb.x1) &&
			        (root->aabb.x2 > item->aabb.x2) &&
			        (root->aabb.y1 < item->aabb.y1) &&
			        (root->aabb.y2 > item->aabb.y2)) {
				root->anyItems(true);
			}

			// We reached the maximum allowed depth which is the leaf node, so add it definitely.
			if(level == depth) {
				item->addNode(root);
				root->addNode(item);
				root->addItem(item->getItem(0));
				root->anyItems(true);
				return;
			}


			T mid_h = root->aabb.y1 + (root->aabb.y2 - root->aabb.y1)/2;
			T mid_v = root->aabb.x1 + (root->aabb.x2 - root->aabb.x1)/2;

			bool upper_quad	= (item->aabb.y1 < mid_h) && (item->aabb.y2 < mid_h);
			bool lower_quad	= (item->aabb.y1 > mid_h);
			bool left_quad 	= (item->aabb.x1 < mid_v) && (item->aabb.x2 < mid_v);
			bool right_quad	= (item->aabb.x1 > mid_v);

			// TODO Shorten this part here.
			if(upper_quad) {
				if(left_quad) {
					insert_recursive(level + 1, root->top_left, item);
				} else if(right_quad) {
					insert_recursive(level + 1, root->top_right, item);
				} else {
					insert_recursive(level + 1, root->top_left, item);
					insert_recursive(level + 1, root->top_right, item);
				}
			} else if(lower_quad) {
				if(left_quad) {
					insert_recursive(level + 1, root->bottom_left, item);
				} else if(right_quad) {
					insert_recursive(level + 1, root->bottom_right, item);
				} else {
					insert_recursive(level + 1, root->bottom_left, item);
					insert_recursive(level + 1, root->bottom_right, item);
				}

			} else {
				if(left_quad) {
					insert_recursive(level + 1, root->top_left, item);
					insert_recursive(level + 1, root->bottom_left, item);
				} else if(right_quad) {
					insert_recursive(level + 1, root->top_right, item);
					insert_recursive(level + 1, root->bottom_right, item);
				} else {
					insert_recursive(level + 1, root->top_left, item);
					insert_recursive(level + 1, root->bottom_left, item);
					insert_recursive(level + 1, root->top_right, item);
					insert_recursive(level + 1, root->bottom_right, item);
				}
			}
			root->anyItems(true);
		}


		NodeType* find(xdl::xdl_int x, xdl::xdl_int y) {
			return find_recursive(0, root, x, y);
		}

		NodeType* find_recursive(xdl::xdl_int level, NodeType* root, xdl::xdl_int x, xdl::xdl_int y) {

			// Do not search in a quadrant if there is nothing or if we reached the leaf node.
			if((level == depth) || !root->anyItems()) {
				return root;
			}

			bool upper_quad = (y < root->aabb.y1 + (root->aabb.y2 - root->aabb.y1)/2);
			bool left_quad = (x < (root->aabb.x1 + (root->aabb.x2 - root->aabb.x1)/2));

			if(upper_quad) {
				// Pointer is in the upper part.
				if(left_quad) {
					if(root->top_left == nullptr) {
						return root;
					} else {
						return find_recursive(level + 1, root->top_left, x, y);
					}
				} else {
					if(root->top_right == nullptr) {
						return root;
					} else {
						return find_recursive(level + 1,root->top_right, x, y);
					}
				}

			} else {
				// Pointer is in bottom part.
				if(left_quad) {
					if(root->bottom_left == nullptr) {
						return root;
					} else {
						return find_recursive(level + 1,root->bottom_left, x, y);
					}
				} else {
					if(root->bottom_right == nullptr) {
						return root;
					} else {
						return find_recursive(level + 1,root->bottom_right, x, y);
					}
				}
			}
			return root;
		}

		void draw() {
			draw_recursive(0, root);
		}


		void create_recursive(xdl::xdl_uint level, NodeType* root) {

			root->split();

			if(level == depth) {
				return;
			}

			create_recursive(level + 1, root->top_left);
			create_recursive(level + 1, root->top_right);
			create_recursive(level + 1, root->bottom_left);
			create_recursive(level + 1, root->bottom_right);
		}

		void delete_recursive(xdl::xdl_int level, NodeType* root) {

			if(level == depth) {
				return;
			}

			delete_recursive(level + 1, root->top_left);
			delete_recursive(level + 1, root->top_right);
			delete_recursive(level + 1, root->bottom_left);
			delete_recursive(level + 1, root->bottom_right);

			delete(root);
		}

		void draw_recursive(int level, NodeType* node) {

			drawNode(node);

			if(level == depth) {
				return;
			}

			draw_recursive(level + 1, node->top_left);
			draw_recursive(level + 1, node->top_right);
			draw_recursive(level + 1, node->bottom_left);
			draw_recursive(level + 1, node->bottom_right);
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

		NodeType* root;
		xdl::xdl_uint width;
		xdl::xdl_uint height;
		xdl::xdl_uint depth;
};

typedef XdevLQuadTree<int, Widget*> QuadTreeType;

static QuadTreeType* widgetSceneSystem;


#endif
