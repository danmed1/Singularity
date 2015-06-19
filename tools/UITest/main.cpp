#include <iostream>
#include <XdevL.h>
#include <XdevLApplication.h>
#include <XdevLOpenGL/XdevLOpenGL.h>

#include <Engine/GUI/CheckBox.h>

struct quadtree_node {
	quadtree_node() : 	top_left(nullptr), 
						top_right(nullptr), 
						bottom_left(nullptr), 
						bottom_right(nullptr),
						x1(0), y1(0), x2(0), y2(0) {}
	quadtree_node* top_left;
	quadtree_node* top_right;
	quadtree_node* bottom_left;
	quadtree_node* bottom_right;
	
	xdl::xdl_int x1, y1, x2, y2;
};

class quadtree {
	quadtree_node* root;
	xdl::xdl_int levels;
	xdl::xdl_uint width;
	xdl::xdl_uint height;
public:
	quadtree(xdl::xdl_int depth, xdl::xdl_uint width, xdl::xdl_uint height) : 
		root(nullptr), 
		levels(depth),
		width(width),
		height(height) {}
	
	void init() {
		create_recursive(0, root, 0, 0, width, height);
	}
	
	void render() {
		draw_recursive(0, root);
	}
	
	void create_recursive(xdl::xdl_uint level, quadtree_node* node, xdl::xdl_int x1, xdl::xdl_int y1, xdl::xdl_int x2, xdl::xdl_int y2 ) {
		if(level == levels) {
			return;
		}
		
		node = new quadtree_node();
		node->x1 = x1;
		node->y1 = y1;
		node->x2 = x2;
		node->y2 = y2;
		
		printf("level: %d, (%d %d %d %d)\n", level, x1, y1, x2, y2);
		
		create_recursive(level + 1, node->top_left,  x1, y1, x2/2, y2/2);
		create_recursive(level + 1, node->top_right, x1 + (x2-x1)/2 , y1, x2, y2/2);
		create_recursive(level + 1, node->bottom_left, x1, y1 + (y2-y1)/2, x2/2, y2);
		create_recursive(level + 1, node->bottom_right, x1 + (x2-x1)/2, y1 + (y2-y1)/2, x2, y2);		
	}
	
	void draw_recursive(xdl::xdl_uint level, quadtree_node* node) {
		if(level == levels) {
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

		printf("level: %d, (%d %d %d %d)\n", level, node->x1, node->y1, node->x2, node->y2);
		
		
		draw_recursive(level + 1, node->top_left);
		draw_recursive(level + 1, node->top_right);
		draw_recursive(level + 1, node->bottom_left);
		draw_recursive(level + 1, node->bottom_right);
	}

	
};

class UITest : public xdl::XdevLApplication {
	public:
		UITest(int argc, char** argv, const char* xml_filename) throw() :
			xdl::XdevLApplication(argc, argv, xdl::XdevLFileName(xml_filename)),
			m_appRun(xdl::xdl_true) {

		}

		virtual void main(const Arguments& argv) throw() override {

			
			quadtree qt(2, 800,400);
			qt.init();
//			return;
			
			initializeRenderSystem();

			m_mouseAxisDelegate = xdl::XdevLAxisDelegateType::Create<UITest, &UITest::mouse_axis_handle>(this);
			getMouse()->registerDelegate(m_mouseAxisDelegate);


			m_buttonDelegate = xdl::XdevLButtonIdDelegateType::Create<UITest, &UITest::escape_key_handle>(this);
			getKeyboard()->registerDelegate(STRING("KEY_ESCAPE"), m_buttonDelegate);


			getWindow()->show();
			getWindow()->setInputFocus();


			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0, getWindow()->getWidth(), getWindow()->getHeight(), 0, -1, 1);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glEnable(GL_DEPTH_TEST);

			while(m_appRun) {
				getCore()->update();

				
				

				glClearColor(0.4f, 0.0f, 0.0f, 0.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				glViewport(0, 0, getWindow()->getWidth(), getWindow()->getHeight());


				glLineWidth(2.5);
				glColor3f(1.0, 1.0, 1.0);
				glBegin(GL_LINES);
				qt.render();
				glEnd();


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
					std::cout << "x: " << value << std::endl;
				}
				break;
				case xdl::AXIS_Y: {
					std::cout << "y: " << value << std::endl;
				}
				break;
				default:
					break;
			}

		}

	private:
		xdl::xdl_bool m_appRun;
		xdl::XdevLButtonIdDelegateType 	m_buttonDelegate;
		xdl::XdevLAxisDelegateType 		m_mouseAxisDelegate;
		xdl::XdevLOpenGL330* 			m_opengl;

};

XdevLStartMain(UITest, "resources/ini/uitest.xml")
