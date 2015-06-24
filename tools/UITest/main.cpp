#include <iostream>
#include <XdevL.h>
#include <XdevLApplication.h>
#include <XdevLOpenGL/XdevLOpenGL.h>


#include <Engine/GUI/Widgets.h>
#include <Engine/TextureServer.h>
#include <Engine/Fonts/XdevLFontImpl.h>
#include <Engine/Fonts/XdevLTextLayoutImpl.h>

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


	private:

		XdevLQuadTreeNode* top_left;
		XdevLQuadTreeNode* top_right;
		XdevLQuadTreeNode* bottom_left;
		XdevLQuadTreeNode* bottom_right;

		AABB aabb;

		xdl::xdl_bool hasItems;

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
		
		void insertObject(OBJ obj) {

			XdevLQuadTreeNode<T, OBJ>* node = new XdevLQuadTreeNode<T, OBJ>();
			node->setAABB(obj->getAABB());
			node->addItem(obj);
			insert(node);
		}

		void insertObjectAll(std::vector<OBJ>& objs) {

			for(auto& obj : objs) {
				insertObject(obj);
			}
		}
		

		void insert(XdevLQuadTreeNode<T, OBJ>* item) {
			insert_recursive(0, root, item);
		}

		void insert_recursive(xdl::xdl_uint level, XdevLQuadTreeNode<T, OBJ>* root, XdevLQuadTreeNode<T, OBJ>* item) {

			// Check if the item fits into this node?
			if((root->aabb.x1 < item->aabb.x1) &&
			        (root->aabb.x2 > item->aabb.x2) &&
			        (root->aabb.y1 < item->aabb.y1) &&
			        (root->aabb.y2 > item->aabb.y2)) {
				root->anyItems(true);
			}

			// We reached the maximum allowed depth which is the leaf node, so add it definitely.
			if(level == depth) {
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


		XdevLQuadTreeNode<T, OBJ>* find(xdl::xdl_int x, xdl::xdl_int y) {
			return find_recursive(0, root, x, y);
		}

		XdevLQuadTreeNode<T, OBJ>* find_recursive(xdl::xdl_int level, XdevLQuadTreeNode<T, OBJ>* root, xdl::xdl_int x, xdl::xdl_int y) {

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


xdl::XdevLTexture* createTextureFromFile(const xdl::xdl_char* filename) {
	return soan::TextureServer::Inst()->import(filename);
}


class UITest : public xdl::XdevLApplication {
	public:
		typedef XdevLQuadTree<int, Widget*> QuadTreeType;

		UITest(int argc, char** argv, const char* xml_filename) throw() :
			xdl::XdevLApplication(argc, argv, xdl::XdevLFileName(xml_filename)),
			m_appRun(xdl::xdl_true),
			m_xaxis(0.0f),
			m_yaxis(0.0f),
			m_font2D(nullptr),
			m_textEngine(nullptr) {

		}
		~UITest() {
			UITest::m_quadtree->shutdown();
			delete UITest::m_quadtree;
		}

		virtual void main(const Arguments& argv) throw() override {

			m_quadtree = new QuadTreeType(0, 0, getWindow()->getWidth(),getWindow()->getHeight(), 3);
			m_quadtree->init();



			initializeRenderSystem();

			m_mouseButtonDelegate = xdl::XdevLButtonDelegateType::Create<UITest, &UITest::mouse_button_handle>(this);
			getMouse()->registerDelegate(m_mouseButtonDelegate);

			m_mouseAxisDelegate = xdl::XdevLAxisDelegateType::Create<UITest, &UITest::mouse_axis_handle>(this);
			getMouse()->registerDelegate(m_mouseAxisDelegate);


			m_buttonDelegate = xdl::XdevLButtonIdDelegateType::Create<UITest, &UITest::escape_key_handle>(this);
			getKeyboard()->registerDelegate(STRING("KEY_ESCAPE"), m_buttonDelegate);


			getWindow()->show();
			getWindow()->setInputFocus();
			getWindow()->grabPointer();



			MenuBar* menuBar = new MenuBar(0, 0, getWindow()->getWidth(), 18);
			menuBar->addMenu(L"File");
			menuBar->addMenu(L"Edit");
			m_quadtree->insertObjectAll(menuBar->getWidgets());


			Button* button1 = new Button(std::wstring(L"File"), 0, 100, 50, 24);
			CheckBox* checkbox1 = new CheckBox(std::wstring(L"Nothing"), 0, 130);
			CheckBox* checkbox2 = new CheckBox(std::wstring(L"Nothing"), 0, 150);
			
			ComboBox* comboBox = new ComboBox(100,100, 100,24);
			comboBox->addItem(L"ViewPort");
			comboBox->addItem(L"LateLane");
			comboBox->addItem(L"TourNome");
			
			m_quadtree->insertObject(button1);
			m_quadtree->insertObject(checkbox1);
			m_quadtree->insertObject(checkbox2);
			m_quadtree->insertObject(comboBox);

			while(m_appRun) {
				getCore()->update();

				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				glOrtho(0, getWindow()->getWidth(), getWindow()->getHeight(), 0, -1, 1);
				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				glDisable(GL_DEPTH_TEST);

				glClearColor(0.33f, 0.32f, 0.30f, 0.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				glViewport(0, 0, getWindow()->getWidth(), getWindow()->getHeight());


			glLineWidth(1.0);
			glColor3f(1.0, 1.0, 1.0);
			glBegin(GL_LINES);
			m_quadtree->draw();
			glEnd();


				glLineWidth(4.0);
				glColor3f(1.0, 0.0, 0.0);
				glBegin(GL_LINES);
				m_quadtree->drawNode(m_currentPointerNode);
				glEnd();

				menuBar->draw();
				button1->draw();
				checkbox1->draw();
				checkbox2->draw();
				comboBox->draw();

//				m_textEngine->setColor(1.0f, 1.0f, 1.0f);
//				m_textEngine->setScale(0.6f);
//				m_textEngine->setEffect(0);
//				xdl::xdl_float x, y;
//				convertWidgetAABBToRelative(Widget::BOTTOM_LEFT, button1->getAABB(), 512, 512, x,y);
//				m_textEngine->addDynamicText(button1->title(), x, y);

;
				
//				m_textEngine->render();

				get3DProcessor()->swapBuffers();
				xdl::sleep(0.002);
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

			soan::TextureServer::Inst();
			soan::TextureServer::Inst()->init(get3DProcessor(), "./");

			m_font2D = new soan::XdevLFontImpl(getWindow(), get3DProcessor());
			m_font2D->setCreateTextureCallback(createTextureFromFile);
			m_font2D->importFromFontFile("resources/fonts/default_info.txt");

			m_textEngine = new soan::XdevLTextLayoutImpl(getWindow(), get3DProcessor());
			m_textEngine->init(m_font2D);

			return xdl::ERR_OK;
		}

	private:
		void escape_key_handle(const xdl::XdevLButtonState& eventType) {
			if(eventType == xdl::BUTTON_RELEASED) {
				m_appRun = false;
			}
		}

		void mouse_button_handle(const xdl::XdevLButtonId& id, const xdl::XdevLButtonState& state) {
			const QuadTreeType::NodeType::NodeItemVectorType& listOfWidgets = m_currentPointerNode->getItems();
			for(auto& i : listOfWidgets) {
				if(state == xdl::BUTTON_PRESSED) {
					i->onButtonPress(id, m_xaxis, m_yaxis);
				} else {
					i->onButtonRelease(id, m_xaxis, m_yaxis);
				}
			}

			switch(id) {
				case xdl::BUTTON_LEFT: {

				}
				break;
				case xdl::BUTTON_RIGHT: {

				}
				break;
				default:
					break;
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
			const QuadTreeType::NodeType::NodeItemVectorType& listOfWidgets =m_currentPointerNode->getItems();
			for(auto& i : listOfWidgets) {
				i->onMouseMove(m_xaxis, m_yaxis);
			}

		}

	private:

		xdl::xdl_bool m_appRun;
		xdl::XdevLButtonIdDelegateType 	m_buttonDelegate;
		xdl::XdevLButtonDelegateType	m_mouseButtonDelegate;
		xdl::XdevLAxisDelegateType 		m_mouseAxisDelegate;
		xdl::XdevLOpenGL330* 			m_opengl;
		static QuadTreeType*			m_quadtree;
		xdl::xdl_float 					m_xaxis;
		xdl::xdl_float					m_yaxis;
		QuadTreeType::NodeType*			m_currentPointerNode;

		soan::XdevLFontImpl*			m_font2D;
		soan::XdevLTextLayoutImpl*		m_textEngine;
};

QuadTreeType UITest::m_quadtree = nullptr;

XdevLStartMain(UITest, "resources/ini/uitest.xml")
