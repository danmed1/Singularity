#include <iostream>
#include <XdevL.h>
#include <XdevLApplication.h>
#include <XdevLRAI/XdevLRAI.h>
#include <XdevLFont/XdevLFont.h>
#include <XdevLFont/XdevLFontSystem.h>
#include <XdevLFont/XdevLTextLayout.h>


#include <Engine/TextureServer.h>

class FontTest : public xdl::XdevLApplication {
	public:


		FontTest(int argc, char** argv, const char* xml_filename) throw() :
			xdl::XdevLApplication(argc, argv, xdl::XdevLFileName(xml_filename)),
			m_appRun(xdl::xdl_true),
			m_font(nullptr),
			m_textEngine(nullptr) {

		}

		~FontTest() {

		}

		virtual void main(const Arguments& argv) throw() override {


			initializeRenderSystem();

			m_mouseAxisDelegate = xdl::XdevLAxisDelegateType::Create<FontTest, &FontTest::mouse_axis_handle>(this);
			getMouse()->registerDelegate(m_mouseAxisDelegate);


			m_buttonDelegate = xdl::XdevLButtonIdDelegateType::Create<FontTest, &FontTest::escape_key_handle>(this);
			getKeyboard()->registerDelegate(STRING("KEY_ESCAPE"), m_buttonDelegate);


			getWindow()->show();
			getWindow()->setInputFocus();


			while(m_appRun) {
				getCore()->update();

				get3DProcessor()->setActiveDepthTest(xdl::xdl_true);
				get3DProcessor()->clearColorTargets(1.0f, 1.0f, 1.0f, 0.0f);
				get3DProcessor()->clearDepthTarget(1.0f);

				m_textEngine->setColor(200, 0, 0, 255);
				m_textEngine->useFont(m_font);
				m_textEngine->printText(L"The quick brown fox jumps over the lazy dog", m_xaxis, m_yaxis);

				m_textEngine->setColor(0, 0, 0, 255);
				m_textEngine->useFont(m_font2);
				m_textEngine->printText(L"The quick brown fox jumps over the lazy dog", 0, 0);

				get3DProcessor()->swapBuffers();
				xdl::sleep(0.002);
			}

		}


		xdl::IPXdevLRAI get3DProcessor() {
			return m_rai;
		}


		xdl::xdl_int initializeRenderSystem() {
			// Get the OpenGL context.
			m_rai = xdl::getModule<xdl::IPXdevLRAI>(getCore(), xdl::XdevLID("MyOpenGL"));
			if(!m_rai) {
				return xdl::ERR_ERROR;
			}

			// We must attach the OpenGL context to a render m_window.
			if(m_rai->create(getWindow()) != xdl::ERR_OK) {
				return xdl::ERR_ERROR;
			}

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

			m_font = m_fontSystem->createFromFontFile(xdl::XdevLFileName("resources/fonts/default_info.txt"));
			m_font2 =  m_fontSystem->createFromFontFile(xdl::XdevLFileName("resources/fonts/Roboto-Regular_info.txt"));

			m_textEngine->init(getWindow(), get3DProcessor());
			m_textEngine->usePixelUnits(xdl::xdl_true);
			m_textEngine->setScale(1.0f);
			m_textEngine->setDFT(0);
			m_textEngine->setEffect(0);

			return xdl::ERR_OK;
		}



	private:
		void escape_key_handle(const xdl::XdevLButtonState& eventType) {
			if(eventType == xdl::BUTTON_RELEASED) {
				m_appRun = false;
			}
		}

		void mouse_axis_handle(const xdl::XdevLAxisId& id, const xdl::xdl_float& value) {
			if(id == xdl::AXIS_0) {
				m_xaxis = value;
			} else if(id == xdl::AXIS_1) {
				m_yaxis = value;
			}
			std::cout << m_xaxis << "x" << m_yaxis << std::endl;
		}

	private:

		xdl::xdl_bool m_appRun;
		xdl::XdevLButtonIdDelegateType 	m_buttonDelegate;
		xdl::XdevLAxisDelegateType 		m_mouseAxisDelegate;
		xdl::IPXdevLRAI 							m_rai;

		xdl::IPXdevLFontSystem		m_fontSystem;
		xdl::IPXdevLFont					m_font;
		xdl::IPXdevLFont					m_font2;
		xdl::IPXdevLTextLayout		m_textEngine;
		xdl::xdl_float m_xaxis;
		xdl::xdl_float m_yaxis;

};


XdevLStartMain(FontTest, "resources/ini/fonttest.xml")
