#include <iostream>
#include <XdevL.h>
#include <XdevLApplication.h>
#include <XdevLOpenGL/XdevLOpenGL.h>


#include <Engine/TextureServer.h>
#include <Engine/Fonts/XdevLFontImpl.h>
#include <Engine/Fonts/XdevLTextLayoutImpl.h>



xdl::XdevLTexture* createTextureFromFile(const xdl::xdl_char* filename) {
	return soan::TextureServer::Inst()->import(filename);
}


class FontTest : public xdl::XdevLApplication {
	public:


		FontTest(int argc, char** argv, const char* xml_filename) throw() :
			xdl::XdevLApplication(argc, argv, xdl::XdevLFileName(xml_filename)),
			m_appRun(xdl::xdl_true),
			m_font2D(nullptr),
			m_textEngine(nullptr) {

		}
		~FontTest() {

		}

		virtual void main(const Arguments& argv) throw() override {


			initializeRenderSystem();

			m_mouseButtonDelegate = xdl::XdevLButtonDelegateType::Create<FontTest, &FontTest::mouse_button_handle>(this);
			getMouse()->registerDelegate(m_mouseButtonDelegate);

			m_mouseAxisDelegate = xdl::XdevLAxisDelegateType::Create<FontTest, &FontTest::mouse_axis_handle>(this);
			getMouse()->registerDelegate(m_mouseAxisDelegate);


			m_buttonDelegate = xdl::XdevLButtonIdDelegateType::Create<FontTest, &FontTest::escape_key_handle>(this);
			getKeyboard()->registerDelegate(STRING("KEY_ESCAPE"), m_buttonDelegate);


			getWindow()->show();
			getWindow()->setInputFocus();
			getWindow()->grabPointer();





			while(m_appRun) {
				getCore()->update();


				glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glViewport(0, 0, getWindow()->getWidth(), getWindow()->getHeight());


				m_textEngine->setColor(0, 0, 0, 255);
				m_textEngine->setScale(1.0f);
				m_textEngine->setDFT(0);
				m_textEngine->setEffect(0);
				m_textEngine->addDynamicText(L"The quick brown fox jumps over the lazy dog", -1, 0.0);
				m_textEngine->addDynamicText(L"Version 1.0.0", -1, 0.2);
				
				m_textEngine->render();



				

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

			getMouse()->setAxisRangeMinMax(xdl::AXIS_0, 0, getWindow()->getWidth());
			getMouse()->setAxisRangeMinMax(xdl::AXIS_1, 0, getWindow()->getHeight());


			return xdl::ERR_OK;
		}



	private:
		void escape_key_handle(const xdl::XdevLButtonState& eventType) {
			if(eventType == xdl::BUTTON_RELEASED) {
				m_appRun = false;
			}
		}

		void mouse_button_handle(const xdl::XdevLButtonId& id, const xdl::XdevLButtonState& state) {

		}

		void mouse_axis_handle(const xdl::XdevLAxisId& id, const xdl::xdl_float& value) {

		}

	private:

		xdl::xdl_bool m_appRun;
		xdl::XdevLButtonIdDelegateType 	m_buttonDelegate;
		xdl::XdevLButtonDelegateType	m_mouseButtonDelegate;
		xdl::XdevLAxisDelegateType 		m_mouseAxisDelegate;
		xdl::XdevLOpenGL330* 			m_opengl;

		soan::XdevLFontImpl*				m_font2D;
		soan::XdevLTextLayoutImpl*		m_textEngine;

};


XdevLStartMain(FontTest, "resources/ini/fonttest.xml")
