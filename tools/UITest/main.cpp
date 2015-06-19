#include <iostream>
#include <XdevL.h>
#include <XdevLApplication.h>
#include <XdevLOpenGL/XdevLOpenGL.h>
#include <Engine/Color.h>

class Widget {
	public:
		Widget(xdl::xdl_int x, xdl::xdl_int y, xdl::xdl_int width, xdl::xdl_int height) :
			x1(x),
			y1(y),
			x2(x + width),
			y2(y + height),
			backgroundColor(soan::Color(0.9f, 0.9f, 0.9f, 1.0f)),
			hoverColor(soan::Color(1.0f, 1.0f, 1.0f, 1.0f)) {}

		void draw(Widget* widget);
		
private:
	
		xdl::xdl_int x1, y1, x2, y2;
		soan::Color backgroundColor;
		soan::Color hoverColor;

};

void Widget::draw(Widget* widget) {

	if(widget == nullptr) {
		return;
	}

	glBegin(GL_TRIANGLE_STRIP);
				
	glColor4f(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
	glVertex2i(widget->x1, widget->y1);
	glVertex2i(widget->x1, widget->y2);
	glVertex2i(widget->x2, widget->y1);
	glVertex2i(widget->x2, widget->y2);

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

		XdevLQuadTreeNode() :
			top_left(nullptr),
			top_right(nullptr),
			bottom_left(nullptr),
			bottom_right(nullptr),
			x1(0), y1(0), x2(0), y2(0) {}

		/// Returns the stored user item of this node.
		const OBJ& getItem() const {
			return item;
		}

		/// Sets the user item for this node.
		void setItem(const OBJ& itm) {
			item = itm;
		}

	private:


		/// Child nodes.

		XdevLQuadTreeNode* top_left;
		XdevLQuadTreeNode* top_right;
		XdevLQuadTreeNode* bottom_left;
		XdevLQuadTreeNode* bottom_right;

		// Holds the (left, top) and (bottom, right) values for the rectangle area.
		T x1, y1, x2, y2;



		// Holds an user defined object.
		OBJ item;
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
		

		XdevLQuadTreeNode<T, OBJ>* find(XdevLQuadTreeNode<T, OBJ>* item) {
			return insert_recursive(root, item);
		}
		
		
		XdevLQuadTreeNode<T, OBJ>* insert_recursive(XdevLQuadTreeNode<T, OBJ>* root, XdevLQuadTreeNode<T, OBJ>* item) {

			
			// Completely inside.
			if( (item->x1 > root->x1) && 
				(item->x2 < root->x2) && 
				(item->y1 > root->y1) && 
				(item->y2 < root->y2)) {
				return root;
			}

			
			return root;
		}
		

		XdevLQuadTreeNode<T, OBJ>* find(xdl::xdl_int x, xdl::xdl_int y) {
			return find_recursive(root, x, y);
		}

		XdevLQuadTreeNode<T, OBJ>* find_recursive(XdevLQuadTreeNode<T, OBJ>* root, xdl::xdl_int x, xdl::xdl_int y) {

			if(y < (root->y1 + (root->y2-root->y1)/2)) {
				// Pointer is in the upper part.
				if(x < (root->x1 + (root->x2-root->x1)/2)) {
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
				if(x < (root->x1 + (root->x2-root->x1)/2)) {
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
			node->x1 = x1;
			node->y1 = y1;
			node->x2 = x2;
			node->y2 = y2;
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

			glVertex2i(node->x1, node->y1);
			glVertex2i(node->x2, node->y1);

			glVertex2i(node->x2, node->y1);
			glVertex2i(node->x2, node->y2);

			glVertex2i(node->x2, node->y2);
			glVertex2i(node->x1, node->y2);

			glVertex2i(node->x1, node->y2);
			glVertex2i(node->x1, node->y1);
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


			m_quadtree = new XdevLQuadTree<int, soan::Color>(0, 0, getWindow()->getWidth(),getWindow()->getHeight(), 4);
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

			Button button(xdl::XdevLString("Press me"), 100, 100, 100, 50);


			while(m_appRun) {
				getCore()->update();

				glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				glViewport(0, 0, getWindow()->getWidth(), getWindow()->getHeight());


//				glLineWidth(1.0);
//				glColor3f(1.0, 1.0, 1.0);
//				glBegin(GL_LINES);
//				m_quadtree->render();
//				glEnd();

				glLineWidth(4.0);
				glColor3f(1.0, 0.0, 0.0);
				glBegin(GL_LINES);
				m_quadtree->drawNode(m_currentPointerNode);
				glEnd();


				button.draw(&button);

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
		}

	private:
		xdl::xdl_bool m_appRun;
		xdl::XdevLButtonIdDelegateType 	m_buttonDelegate;
		xdl::XdevLAxisDelegateType 		m_mouseAxisDelegate;
		xdl::XdevLOpenGL330* 			m_opengl;
		XdevLQuadTree<int, soan::Color>* 		m_quadtree;
		xdl::xdl_float 					m_xaxis;
		xdl::xdl_float					m_yaxis;
		XdevLQuadTreeNode<int, soan::Color>*	m_currentPointerNode;

};

XdevLStartMain(UITest, "resources/ini/uitest.xml")
