#include <iostream>
#include <XdevL.h>
#include <XdevLApplication.h>
#include <XdevLOpenGL/XdevLOpenGL.h>
#include <XdevLFont/XdevLFont.h>
#include <XdevLFont/XdevLFontSystem.h>
#include <XdevLFont/XdevLTextLayout.h>

#include <Engine/GUI/CanvasXdevLOpenGL.h>
#include <Engine/GUI/CheckBox.h>
#include <Engine/GUI/ComboBox.h>
#include <Engine/GUI/WidgetSceneSystem.h>
#include <Engine/GUI/VBox.h>

#include <Engine/TextureServer.h>


class UITest : public xdl::XdevLApplication {
	public:


		UITest(int argc, char** argv, const char* xml_filename) throw() :
			xdl::XdevLApplication(argc, argv, xdl::XdevLFileName(xml_filename)),
			m_appRun(xdl::xdl_true),
			m_font(nullptr),
			m_textEngine(nullptr) {

			getCore()->registerListener(this);

		}

		~UITest() {
			widgetSceneSystem->shutdown();
			delete widgetSceneSystem;

			delete m_font;
		}

		virtual void main(const Arguments& argv) throw() override {

			widgetSceneSystem = new WidgetSceneSystem(getCore());
			widgetSceneSystem->init(getWindow()->getWidth(), getWindow()->getHeight());

			initializeRenderSystem();

			m_mouseButtonDelegate = xdl::XdevLButtonDelegateType::Create<UITest, &UITest::mouse_button_handle>(this);
			getMouse()->registerDelegate(m_mouseButtonDelegate);

			m_mouseAxisDelegate = xdl::XdevLAxisDelegateType::Create<UITest, &UITest::mouse_axis_handle>(this);
			getMouse()->registerDelegate(m_mouseAxisDelegate);


			m_buttonDelegate = xdl::XdevLButtonIdDelegateType::Create<UITest, &UITest::escape_key_handle>(this);
			getKeyboard()->registerDelegate(STRING("KEY_Q"), m_buttonDelegate);


			getWindow()->show();
			getWindow()->setInputFocus();
			getWindow()->grabPointer();


			canvas = new soan::CanvasXdevLOpenGL(getWindow()->getWidth(), getWindow()->getHeight(), m_textEngine, m_opengl);
			widgetSceneSystem->setCanvas(canvas);

			WidgetSceneSystem::QuadTreeType::DrawNodeDelegateType delegate = WidgetSceneSystem::QuadTreeType::DrawNodeDelegateType::Create<UITest, &UITest::drawGrid>(this);
			widgetSceneSystem->setDrawNodeCallbackType(delegate);


//			MenuBar* menuBar = new MenuBar(0, 0, getWindow()->getWidth(), 24);
//			ContextMenu::OnItemSelectedDelegateType fileMenuDelegate = ContextMenu::OnItemSelectedDelegateType::Create<UITest, &UITest::onFileItemSelected>(this);
//			ContextMenu* menu1 = menuBar->addMenu(L"File");
//
//			menu1->bindOnItemSelected(fileMenuDelegate);
//			ContextMenu* menu2 = menuBar->addMenu(L"Help");
//			menu1->addItem(L"New File");
//			menu1->addItem(L"Open File");
//			menu1->addItem(L"Quit");
//
//			menu2->addItem(L"About");


//			// Create a Button.
//			Button* button1 = new Button(std::wstring(L"Press Me"), 0, 280, 70, 24);
//			button1->setCanvas(canvas);
//
//			// Create the delegate that will handle the users mouse button click on it.
//			Widget::OnClickedDelegate quitDelegate = Widget::OnClickedDelegate::Create<UITest, &UITest::onQuitClicked>(this);
//			button1->bindOnClicked(quitDelegate);
//
//			// Register this Button to the system.
//			widgetSceneSystem->registerWidget(button1);


//			// Create a CheckBox.
//			CheckBox* checkbox1 = new CheckBox(std::wstring(L"Fullscreen"), 0, 130);
//			checkbox1->setCanvas(canvas);
//
//			CheckBox::OnCheckStateDelegateType checkDelegate = CheckBox::OnCheckStateDelegateType::Create<UITest, &UITest::onCheckedBox>(this);
//			checkbox1->bindOnCheck(checkDelegate);
//
//			// Register this CheckBox to the system.
//			widgetSceneSystem->registerWidget(checkbox1);


			// Create a ComboBox.
			ComboBox* comboBox = new ComboBox(0, getWindow()->getHeight() - 24, 100,24);
			comboBox->setCanvas(canvas);

			// Add Items into the ComboBox.
			comboBox->addItem(L"Fullsreen", 0);
			comboBox->addItem(L"Windowed", 1);
			comboBox->addItem(L"View", 2);
			comboBox->addItem(L"Search", 3);
			comboBox->addItem(L"Workspace", 4);
			comboBox->addItem(L"Help", 5);
			comboBox->addItem(L"Quit", 6);

			comboBox->selectItem(4);


			// Create the delegate that will handle user selection in the ComboBox.
			ComboBox::OnItemSelectedDelegateType fileDelegate = ComboBox::OnItemSelectedDelegateType::Create<UITest, &UITest::onItemSelected>(this);
			comboBox->bindOnItemSelected(fileDelegate);

			// Register this ComboBox to the system.
			widgetSceneSystem->registerWidget(comboBox);


//			VBox* vbox = new VBox();
//			vbox->addChild(comboBox);
//			vbox->addChild(checkbox1);

//			widgetSceneSystem->registerWidget(menuBar);




			while(m_appRun) {
				soan::CanvasScope cvs(canvas, getWindow());

				getCore()->update();

				canvas->clearColorBuffer();
				glViewport(0, 0, getWindow()->getWidth(), getWindow()->getHeight());

				widgetSceneSystem->drawGrid();
				widgetSceneSystem->drawNode(widgetSceneSystem->getCurrentPointerNode());


				widgetSceneSystem->draw();

				canvas->render();

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


			// Get the FontSystem
			m_fontSystem = xdl::getModule<xdl::XdevLFontSystem*>(getCore(), xdl::XdevLID("MyFontSystem"));
			if(!m_fontSystem) {
				return xdl::ERR_ERROR;
			}

			// Get the Text Layout System.
			m_textEngine = xdl::getModule<xdl::XdevLTextLayout*>(getCore(), xdl::XdevLID("MyTextLayout"));
			if(!m_textEngine) {
				return xdl::ERR_ERROR;
			}

			// Initialize font system.
			m_fontSystem->init(getWindow()->getWidth(), getWindow()->getHeight(), get3DProcessor());

			m_font = m_fontSystem->createFromFontFile("resources/fonts/default_info.txt");

			m_textEngine->init(getWindow()->getWidth(), getWindow()->getHeight(), get3DProcessor());
			m_textEngine->usePixelUnits(xdl::xdl_true);
			m_textEngine->setScale(1.0f);
			m_textEngine->setDFT(0);
			m_textEngine->setEffect(0);
			m_textEngine->useFont(m_font);

			getMouse()->setAxisRangeMinMax(xdl::AXIS_0, 0, getWindow()->getWidth());
			getMouse()->setAxisRangeMinMax(xdl::AXIS_1, getWindow()->getHeight(), 0);


			return xdl::ERR_OK;
		}

		void drawGrid(WidgetSceneSystem::QuadTreeType::NodeType* node) {
			const AABB& aabb = node->getAABB();

			canvas->drawLine(aabb.x1, aabb.y1, aabb.x1, aabb.y2);
			canvas->drawLine(aabb.x1, aabb.y2, aabb.x2, aabb.y2);
			canvas->drawLine(aabb.x2, aabb.y2, aabb.x2, aabb.y1);
			canvas->drawLine(aabb.x2, aabb.y1, aabb.x1, aabb.y1);
		}

		void onCheckedBox(const CheckBox::State& state, Widget* widget) {
			std::wcout << L"CheckBox got " << ((state == CheckBox::CHECKED) ? L"Checked" : L"Unchecked") << std::endl;
		}

		void onFileItemSelected(Widget* widget) {
			std::wcout << L"File item: " << widget->getTitle() << L" selected." << std::endl;
		}


		void onItemSelected(xdl::xdl_uint id, Widget* widget) {
			switch(id) {
				case 0: {
					getWindow()->setSize(xdl::XdevLWindowSize(1024, 1024));

//					getWindow()->setFullscreen(xdl::xdl_true);
				}
				break;
				case 1: {
				//	getWindow()->setFullscreen(xdl::xdl_false);
				}
				break;
				case 6: {
					m_appRun = xdl::xdl_false;
				}
			}

			std::wcout << L"ComboBox item: " << id << " title = " << widget->getTitle() << L" selected." << std::endl;
		}

		void onQuitClicked(Widget* widget) {
			std::cout << "You clicked the quit button" << std::endl;
		}

		virtual xdl::xdl_int notify(xdl::XdevLEvent& event) {
			switch(event.type) {
				case xdl::XDEVL_CORE_EVENT: {
					if(event.core.type == xdl::XDEVL_CORE_SHUTDOWN) {
						m_appRun = xdl::xdl_false;
					}
				}
				break;
				case xdl::XDEVL_WINDOW_EVENT: {
					switch(event.window.event) {
						case xdl::XDEVL_WINDOW_INPUT_FOCUS_GAINED: {
							widgetSceneSystem->onFocusLost(event.window.windowid);
						}break;
						case xdl::XDEVL_WINDOW_RESIZED: {
							widgetSceneSystem->init(getWindow()->getWidth(), getWindow()->getHeight());
							m_textEngine->init(getWindow()->getWidth(), getWindow()->getHeight(), get3DProcessor());
							canvas->setDimensions(getWindow()->getWidth(), getWindow()->getHeight());
							getMouse()->setAxisRangeMinMax(xdl::AXIS_0, 0, getWindow()->getWidth());
							getMouse()->setAxisRangeMinMax(xdl::AXIS_1, getWindow()->getHeight(), 0);
						}
						break;
					}
				}
				break;
			}
			return xdl::ERR_OK;
		}

	private:
		void escape_key_handle(const xdl::XdevLButtonState& eventType) {
			if(eventType == xdl::BUTTON_RELEASED) {
				m_appRun = false;
			}
		}

		void mouse_button_handle(const xdl::XdevLButtonId& id, const xdl::XdevLButtonState& state) {
			widgetSceneSystem->onButton(id, state);
		}

		void mouse_axis_handle(const xdl::XdevLAxisId& id, const xdl::xdl_float& value) {
			widgetSceneSystem->onPointerMotion(id, value);
		}

	private:

		xdl::xdl_bool m_appRun;
		xdl::XdevLButtonIdDelegateType 	m_buttonDelegate;
		xdl::XdevLButtonDelegateType	m_mouseButtonDelegate;
		xdl::XdevLAxisDelegateType 		m_mouseAxisDelegate;
		xdl::XdevLOpenGL330* 			m_opengl;

		xdl::XdevLFontSystem*			m_fontSystem;
		xdl::XdevLFont*					m_font;
		xdl::XdevLTextLayout*			m_textEngine;

		WidgetSceneSystem* widgetSceneSystem;
		soan::Canvas* canvas;
};


XdevLStartMain(UITest, "resources/ini/uitest.xml")
