#include <iostream>
#include <XdevL.h>
#include <XdevLApplication.h>
#include <XdevLOpenGL/XdevLOpenGL.h>
#include <Engine/Color.h>


class AABB {
	public:
		AABB() :
			x1(0),
			y1(0),
			x2(0),
			y2(0) {}

		AABB(xdl::xdl_int _x1, xdl::xdl_int _y1, xdl::xdl_int _x2, xdl::xdl_int _y2) :
			x1(_x1),
			y1(_y1),
			x2(_x2),
			y2(_y2) {}
			
		xdl::xdl_int x1, y1, x2, y2;
};

class Widget {
	public:
		Widget(xdl::xdl_int x, xdl::xdl_int y, xdl::xdl_int width, xdl::xdl_int height) :
			aabb(x, y, x + width, y + height),
			backgroundColor(soan::Color(0.9f, 0.9f, 0.9f, 1.0f)),
			hoverColor(soan::Color(1.0f, 1.0f, 1.0f, 1.0f)) {}

		void draw();

		const AABB& getAABB() const {
			return aabb;
		}
	private:

		AABB aabb;

		soan::Color backgroundColor;
		soan::Color hoverColor;

};

void Widget::draw() {

	Widget* widget = this;

	glBegin(GL_TRIANGLE_STRIP);

	glColor4f(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);

	glVertex2i(widget->aabb.x1, widget->aabb.y1);
	glVertex2i(widget->aabb.x1, widget->aabb.y2);
	glVertex2i(widget->aabb.x2, widget->aabb.y1);
	glVertex2i(widget->aabb.x2, widget->aabb.y2);

	glEnd();
}

class Button : public Widget {
	public:
		Button(const xdl::XdevLString& title, xdl::xdl_int x, xdl::xdl_int y, xdl::xdl_int width, xdl::xdl_int height) :
			Widget(x, y, width, height),
			text(title) {}

		const xdl::XdevLString& title() const {
			return text;
		}

	private:
		xdl::XdevLString text;
};



template<typename T, typename OBJ>
class XdevLQuadTree;

template<typename T, typename OBJ>
struct XdevLQuadTreeNode {
		friend class XdevLQuadTree<T, OBJ>;

		XdevLQuadTreeNode(const AABB& _aabb) : aabb(_aabb) {} 

		XdevLQuadTreeNode() :
			top_left(nullptr),
			top_right(nullptr),
			bottom_left(nullptr),
			bottom_right(nullptr) {}

		/// Returns the stored user item of this node.
		const OBJ& getItem(xdl::xdl_int idx) const {
			return items[idx];
		}

		const std::vector<OBJ>& getItems() const {
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


		void split() {
			top_left		= new XdevLQuadTreeNode<T, OBJ>(AABB(aabb.x1, aabb.y1, aabb.x1 + (aabb.x2-aabb.x1)/2, aabb.y1 +(aabb.y2-aabb.y1)/2));
			top_right		= new XdevLQuadTreeNode<T, OBJ>(AABB(aabb.x1 + (aabb.x2-aabb.x1)/2 , aabb.y1, aabb.x2, aabb.y1 + (aabb.y2-aabb.y1)/2));
			bottom_left		= new XdevLQuadTreeNode<T, OBJ>(AABB(aabb.x1, aabb.y1 + (aabb.y2-aabb.y1)/2, aabb.x1 + (aabb.x2-aabb.x1)/2, aabb.y2));
			bottom_right	= new XdevLQuadTreeNode<T, OBJ>(AABB(aabb.x1 + (aabb.x2-aabb.x1)/2, aabb.y1 + (aabb.y2-aabb.y1)/2, aabb.x2, aabb.y2));
		}


	private:
		AABB aabb;

		/// Child nodes.

		XdevLQuadTreeNode* top_left;
		XdevLQuadTreeNode* top_right;
		XdevLQuadTreeNode* bottom_left;
		XdevLQuadTreeNode* bottom_right;

		// Holds an user defined object.
		std::vector<OBJ> items;
};

template<typename T, typename OBJ>
class XdevLQuadTree {
	public:
		typedef XdevLQuadTreeNode<T, OBJ> NodeType;

		XdevLQuadTree(xdl::xdl_int x, xdl::xdl_int y, xdl::xdl_uint width, xdl::xdl_uint height, xdl::xdl_uint depth) :
			root(nullptr),
			width(width),
			height(height),
			depth(depth),
			top_left_x(x),
			top_left_y(y) {}
		~XdevLQuadTree() {

		}

		void init() {
			create_recursive(0, &root, top_left_x, top_left_y, width, height);
		}

		void shutdown() {
			delete_recursive(root);
			root = nullptr;
		}


		void insert(XdevLQuadTreeNode<T, OBJ>* item) {
			insert_recursive(0 + 1, root, item);
		}

		void insert_recursive(xdl::xdl_uint level, XdevLQuadTreeNode<T, OBJ>* root, XdevLQuadTreeNode<T, OBJ>* item) {
			
			T mid_h = root->aabb.y1 + (root->aabb.y2 - root->aabb.y1)/2;
			T mid_v = root->aabb.x1 + (root->aabb.x2 - root->aabb.x1)/2;

			bool upper_quad	= (item->aabb.y1 < mid_h) && (item->aabb.y2 < mid_h);
			bool lower_quad	= (item->aabb.y1 > mid_h);
			bool left_quad 	= (item->aabb.x1 < mid_v) && (item->aabb.x2 < mid_v);
			bool right_quad	= (item->aabb.x1 > mid_v);

			if(upper_quad) {
				if(left_quad) {
					if(level == depth - 1) {
						root->top_left->addItem(item->getItem(0));
					} else {
						insert_recursive(level + 1, root->top_left, item);
					}
					return;
				} else if(right_quad) {
					if(level == depth - 1) {
						root->top_right->addItem(item->getItem(0));
					} else {
						insert_recursive(level + 1, root->top_right, item);
					}
					return;
				}
				// It seems that the items borders intersect with the mid vertical line of the root broders.

			} else if(lower_quad) {
				if(left_quad) {
					if(level == depth - 1) {
						root->bottom_left->addItem(item->getItem(0));
					} else {
						insert_recursive(level + 1, root->bottom_left, item);
					}
					return;
				} else if(right_quad) {
					if(level == depth - 1) {
						root->bottom_right->addItem(item->getItem(0));
						return;
					} else {
						item->aabb.x1 = mid_v;
						insert_recursive(level + 1, root->bottom_right, item);
						return;
					}
				}
				// It seems that the items borders intersect with the mid vertical line of the root broders.
			}
			
			NodeType* item1 = new NodeType();
			NodeType* item2 = new NodeType();
			
			AABB aabb1 = item->getAABB();
			aabb1.y2 = mid_h;
			aabb1.x2 = mid_v;
			item1->setAABB(aabb1);
			item1->addItem(item->getItem(0));
			
			AABB aabb2 = item->getAABB();
			aabb2.x1 = mid_v + 1;
			item2->setAABB(aabb2);
			item2->addItem(item->getItem(0));
			
			if(left_quad) {
				insert_recursive(level + 1, root->top_left, item1);
				insert_recursive(level + 1, root->bottom_left, item2);
				return;
			} else if(right_quad) {
				insert_recursive(level + 1, root->top_right, item1);
				insert_recursive(level + 1, root->bottom_right, item2);
				return;
			} 
			

			NodeType* item3 = new NodeType();
			NodeType* item4 = new NodeType();
			
			AABB aabb3 = item->getAABB();
			aabb3.y1 = mid_h;
			aabb3.x2 = mid_v;
			item3->setAABB(aabb3);
			item3->addItem(item->getItem(0));
			
			AABB aabb4 = item->getAABB();
			aabb4.x1 = mid_v + 1;
			aabb4.y1 = mid_h;
			item4->setAABB(aabb4);
			item4->addItem(item->getItem(0));


			insert_recursive(level + 1, root->top_left, item1);
			insert_recursive(level + 1, root->bottom_left, item3);
			insert_recursive(level + 1, root->top_right, item2);
			insert_recursive(level + 1, root->bottom_right, item4);

		}


		XdevLQuadTreeNode<T, OBJ>* find(xdl::xdl_int x, xdl::xdl_int y) {
			return find_recursive(root, x, y);
		}

		XdevLQuadTreeNode<T, OBJ>* find_recursive(XdevLQuadTreeNode<T, OBJ>* root, xdl::xdl_int x, xdl::xdl_int y) {

			bool upper_quad = (y < root->aabb.y1 + (root->aabb.y2 - root->aabb.y1)/2);
			bool left_quad = (x < (root->aabb.x1 + (root->aabb.x2 - root->aabb.x1)/2));

			if(upper_quad) {
				// Pointer is in the upper part.
				if(left_quad) {
					if(root->top_left == nullptr) {
						return root;
					} else {
						return find_recursive(root->top_left, x, y);
					}
				} else {
					if(root->top_right == nullptr) {
						return root;
					} else {
						return find_recursive(root->top_right, x, y);
					}
				}

			} else {
				// Pointer is in bottom part.
				if(left_quad) {
					if(root->bottom_left == nullptr) {
						return root;
					} else {
						return find_recursive(root->bottom_left, x, y);
					}
				} else {
					if(root->bottom_right == nullptr) {
						return root;
					} else {
						return find_recursive(root->bottom_right, x, y);
					}
				}
			}
			return root;
		}

		void render() {
			draw_recursive(root);
		}


		void create_recursive(xdl::xdl_uint level, NodeType** root, xdl::xdl_int x1, xdl::xdl_int y1, xdl::xdl_int x2, xdl::xdl_int y2) {
			if(level == depth) {
				return;
			}

			NodeType* node = new NodeType();
			node->aabb.x1 = x1;
			node->aabb.y1 = y1;
			node->aabb.x2 = x2;
			node->aabb.y2 = y2;
			*root = node;

			create_recursive(level + 1, &node->top_left,  x1, y1, x1 + (x2-x1)/2, y1 +(y2-y1)/2);
			create_recursive(level + 1, &node->top_right, x1 + (x2-x1)/2 , y1, x2, y1 + (y2-y1)/2);
			create_recursive(level + 1, &node->bottom_left, x1, y1 + (y2-y1)/2, x1 + (x2-x1)/2, y2);
			create_recursive(level + 1, &node->bottom_right, x1 + (x2-x1)/2, y1 + (y2-y1)/2, x2, y2);
		}

		void delete_recursive(NodeType* root) {
			if(root == nullptr) {
				return;
			}

			delete_recursive(root->top_left);
			delete_recursive(root->top_right);
			delete_recursive(root->bottom_left);
			delete_recursive(root->bottom_right);

			delete(root);
		}

		void draw_recursive(NodeType* node) {
			if(node == nullptr) {
				return;
			}

			drawNode(node);

			draw_recursive(node->top_left);
			draw_recursive(node->top_right);
			draw_recursive(node->bottom_left);
			draw_recursive(node->bottom_right);
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
		T top_left_x;
		T top_left_y;
};


class UITest : public xdl::XdevLApplication {
	public:
		typedef XdevLQuadTree<int, Widget*> QuadTreeType;

		UITest(int argc, char** argv, const char* xml_filename) throw() :
			xdl::XdevLApplication(argc, argv, xdl::XdevLFileName(xml_filename)),
			m_appRun(xdl::xdl_true),
			m_xaxis(0.0f),
			m_yaxis(0.0f) {

		}
		~UITest() {
			m_quadtree->shutdown();
			delete m_quadtree;
		}

		virtual void main(const Arguments& argv) throw() override {


			m_quadtree = new QuadTreeType(0, 0, getWindow()->getWidth(),getWindow()->getHeight(), 4);
			m_quadtree->init();



			initializeRenderSystem();

			m_mouseAxisDelegate = xdl::XdevLAxisDelegateType::Create<UITest, &UITest::mouse_axis_handle>(this);
			getMouse()->registerDelegate(m_mouseAxisDelegate);


			m_buttonDelegate = xdl::XdevLButtonIdDelegateType::Create<UITest, &UITest::escape_key_handle>(this);
			getKeyboard()->registerDelegate(STRING("KEY_ESCAPE"), m_buttonDelegate);


			getWindow()->show();
			getWindow()->setInputFocus();
			getWindow()->grabPointer();


			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0, getWindow()->getWidth(), getWindow()->getHeight(), 0, -1, 1);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glEnable(GL_DEPTH_TEST);

			Button* button1 = new Button(xdl::XdevLString("Press me"), 100, 100, 100, 50);
			Button* button2 = new Button(xdl::XdevLString("Press me too"), 100, 300, 100, 50);

			QuadTreeType::NodeType* node1 = new QuadTreeType::NodeType();
			QuadTreeType::NodeType* node2 = new QuadTreeType::NodeType();
			node1->setAABB(button1->getAABB());
			node1->addItem(button1);
			
			node2->setAABB(button2->getAABB());
			node2->addItem(button2);
//			m_quadtree->insert(node1);
		//	m_quadtree->insert(node2);

			while(m_appRun) {
				getCore()->update();

				glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				glViewport(0, 0, getWindow()->getWidth(), getWindow()->getHeight());


				glLineWidth(1.0);
				glColor3f(1.0, 1.0, 1.0);
				glBegin(GL_LINES);
				m_quadtree->render();
				glEnd();

				
					glLineWidth(4.0);
					glColor3f(1.0, 0.0, 0.0);
					glBegin(GL_LINES);
					m_quadtree->drawNode(m_currentPointerNode);
					glEnd();


				button1->draw();
				button2->draw();

				get3DProcessor()->swapBuffers();
			}

		}


		xdl::XdevLOpenGL330* get3DProcessor() {
			return m_opengl;
		}


		xdl::xdl_int initializeRenderSystem() {
			// Get the OpenGL context.
			m_opengl = xdl::getModule<xdl::XdevLOpenGL330*>(getCore(), xdl::XdevLID("MyOpenGL"));
			if(!m_opengl) {
				return xdl::ERR_ERROR;
			}

			// We must attach the OpenGL context to a render m_window.
			if(m_opengl->createContext(getWindow()) != xdl::ERR_OK) {
				return xdl::ERR_ERROR;
			}

			return xdl::ERR_OK;
		}

	private:
		void escape_key_handle(const xdl::XdevLButtonState& eventType) {
			if(eventType == xdl::BUTTON_RELEASED) {
				m_appRun = false;
			}
		}

		void mouse_axis_handle(const xdl::XdevLAxisId& id, const xdl::xdl_float& value) {
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
			m_currentPointerNode = m_quadtree->find(m_xaxis, m_yaxis);
			for(auto& i : m_currentPointerNode->getItems()) {
				const AABB& widget = i->getAABB();
				std::cout << "Title: [" << widget.x1 << ", " << widget.y1 << "]" << std::endl;
			}
		}

	private:

		xdl::xdl_bool m_appRun;
		xdl::XdevLButtonIdDelegateType 	m_buttonDelegate;
		xdl::XdevLAxisDelegateType 		m_mouseAxisDelegate;
		xdl::XdevLOpenGL330* 			m_opengl;
		QuadTreeType* 					m_quadtree;
		xdl::xdl_float 					m_xaxis;
		xdl::xdl_float					m_yaxis;
		QuadTreeType::NodeType*			m_currentPointerNode;

};

XdevLStartMain(UITest, "resources/ini/uitest.xml")
