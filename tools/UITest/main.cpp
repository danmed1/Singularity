#include <iostream>
#include <XdevL.h>
#include <XdevLApplication.h>
#include <XdevLOpenGL/XdevLOpenGL.h>


#include <Engine/GUI/Widgets.h>
#include <Engine/GUI/WidgetSceneSystem.h>

#include <Engine/TextureServer.h>
#include <Engine/Fonts/XdevLFontImpl.h>
#include <Engine/Fonts/XdevLTextLayoutImpl.h>





xdl::XdevLTexture* createTextureFromFile(const xdl::xdl_char* filename) {
	return soan::TextureServer::Inst()->import(filename);
}


class UITest : public xdl::XdevLApplication {
	public:


		UITest(int argc, char** argv, const char* xml_filename) throw() :
			xdl::XdevLApplication(argc, argv, xdl::XdevLFileName(xml_filename)),
			m_appRun(xdl::xdl_true),
			m_xaxis(0.0f),
			m_yaxis(0.0f),
			m_font2D(nullptr),
			m_textEngine(nullptr) {

		}
		~UITest() {
			widgetSceneSystem->shutdown();
			delete widgetSceneSystem;
		}

		virtual void main(const Arguments& argv) throw() override {

			widgetSceneSystem = new QuadTreeType(0, 0, getWindow()->getWidth(),getWindow()->getHeight(), 3);
			widgetSceneSystem->init();



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
			widgetSceneSystem->insertObjectAll(menuBar->getWidgets());


			Button* button1 = new Button(std::wstring(L"File"), 0, 100, 50, 24);
			CheckBox* checkbox1 = new CheckBox(std::wstring(L"Nothing"), 0, 130);
			CheckBox* checkbox2 = new CheckBox(std::wstring(L"Nothing"), 0, 150);
			
			ComboBox* comboBox = new ComboBox(100,100, 100,24);
			comboBox->addItem(L"ViewPort");
			comboBox->addItem(L"LateLane");
			comboBox->addItem(L"TourNome");
			widgetSceneSystem->insertObjectAll(comboBox->getWidgets());
			
			
			widgetSceneSystem->insertObject(button1);
			widgetSceneSystem->insertObject(checkbox1);
			widgetSceneSystem->insertObject(checkbox2);
			widgetSceneSystem->insertObject(comboBox);

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
			widgetSceneSystem->draw();
			glEnd();


				glLineWidth(4.0);
				glColor3f(1.0, 0.0, 0.0);
				glBegin(GL_LINES);
				widgetSceneSystem->drawNode(m_currentPointerNode);
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
			m_currentPointerNode = widgetSceneSystem->find(m_xaxis, m_yaxis);
			std::cout << m_currentPointerNode->getNumberOfAssigedNodes() << std::endl;
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
		xdl::xdl_float 					m_xaxis;
		xdl::xdl_float					m_yaxis;
		QuadTreeType::NodeType*			m_currentPointerNode;

		soan::XdevLFontImpl*			m_font2D;
		soan::XdevLTextLayoutImpl*		m_textEngine;
};


XdevLStartMain(UITest, "resources/ini/uitest.xml")
