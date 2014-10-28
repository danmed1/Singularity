#include "SplashScreen.h"
#include <XdevLWindow/XdevLWindow.h>

#include "Engine/TextureServer.h"
#include "Engine/Fonts/XdevLFontImpl.h"
#include "Engine/Fonts/XdevLTextLayoutImpl.h"

namespace soan {

	namespace utils {

		xdl::XdevLTexture* createTextureFromFile(const xdl::xdl_char* filename) {
			return soan::TextureServer::Inst()->import(filename);
		}

		SplashScreen::SplashScreen(xdl::IPXdevLCore core) :
			m_core(core),
			m_running(xdl::xdl_true) {
		}

		SplashScreen::~SplashScreen() {
		}

		xdl::XdevLID* SplashScreen::getID() {
			static xdl::XdevLID SplashScreenID("SplashScreen");
			return &SplashScreenID;
		}

		xdl::xdl_int SplashScreen::notify(xdl::XdevLEvent& event) {

			return 0;
		}

		void SplashScreen::show() {
			Start();
		}

		void SplashScreen::hide() {
			m_mutex.Lock();
			m_running = xdl::xdl_false;
			m_mutex.Unlock();
		}

		xdl::xdl_bool SplashScreen::getRunningState() {
			xdl::xdl_bool tmp;
			m_mutex.Lock();
			tmp = m_running;
			m_mutex.Unlock();
			return tmp;
		}

		int SplashScreen::RunThread(thread::ThreadArgument* p_arg) {
			xdl::IPXdevLWindow window = xdl::createModule<xdl::IPXdevLWindow>(m_core, "XdevLWindow", "SplashWindow");

			xdl::XdevLOpenGL330* m_openGL = static_cast<xdl::XdevLOpenGL330*>(m_core->createModule("XdevLOpenGL", "SplashScreenContext"));
			m_openGL->createContext(window);
			m_openGL->makeCurrent(window);

			soan::XdevLFontImpl* fontEngine = new soan::XdevLFontImpl(window, m_openGL);
			soan::TextureServer::Inst()->setResourcePathPrefix("./");

			fontEngine->setCreateTextureCallback(createTextureFromFile);
			fontEngine->importFromFontFile("resources/fonts/default_info.txt");

			soan::XdevLTextLayout* textLayouter = new soan::XdevLTextLayoutImpl(window, m_openGL);
			textLayouter->init(fontEngine);


			xdl::XdevLVertexShader* vs;
			m_openGL->createVertexShader(&vs);
			xdl::XdevLFragmentShader* fs;
			m_openGL->createFragmentShader(&fs);
			xdl::XdevLShaderProgram* sp;
			m_openGL->createShaderProgram(&sp);

			if(vs->compileFromFile("resources/shaders/splashscreen/splash_vs.glsl") != xdl::ERR_OK) {
				return xdl::ERR_ERROR;
			}

			if(fs->compileFromFile("resources/shaders/splashscreen/splash_fs.glsl") != xdl::ERR_OK) {
				return xdl::ERR_ERROR;
			}

			sp->attach(vs);
			sp->attach(fs);
			sp->link();

			xdl::xdl_float screen_vertex [] = {
				-1.0, -1.0f,
				-1.0f, 1.0f,
				1.0f, 1.0f,

				1.0f, 1.0f,
				1.0f, -1.0f,
				-1.0f, -1.0f
			};


			xdl::XdevLVertexDeclaration* vd = new xdl::XdevLVertexDeclaration();
			vd->add(2, xdl::XDEVL_BUFFER_ELEMENT_FLOAT, 0);


			std::vector<xdl::xdl_uint8*> list;
			list.push_back((xdl::xdl_uint8*)screen_vertex);

			xdl::XdevLVertexArray* va;
			m_openGL->createVertexArray(&va);
			if(va->init(list.size(), list.data(), 6, vd) != xdl::ERR_OK) {
				return -1;
			}

			while(true) {

				if(!getRunningState()) {
					break;
				}

				glClearColor(1.0f, 0.0, 0.0, 1.0);
				glClear(GL_COLOR_BUFFER_BIT);


				m_openGL->setActiveShaderProgram(sp);
				m_openGL->setActiveVertexArray(va);

				m_openGL->drawVertexArray(xdl::XDEVL_PRIMITIVE_TRIANGLES, 6);

				
				std::wstring maxVertices = L"Singularity";
				textLayouter->setScale(1.0f);
				textLayouter->setColor(1.0f, 1.0f, 1.0f);
				textLayouter->addDynamicText(maxVertices.c_str(), -0.10, 0.0);

				textLayouter->render();

				m_openGL->swapBuffers();

				xdl::sleep(0.100);
			}

			xdl::destroyModule(m_core, window);
			return 0;
		}

	}

}