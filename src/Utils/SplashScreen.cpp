#include "SplashScreen.h"

#include "Engine/TextureServer.h"

namespace soan {

	namespace utils {

		xdl::XdevLTexture* createTextureFromFile(const xdl::xdl_char* filename) {
			return soan::TextureServer::Inst()->import(filename);
		}

		SplashScreen::SplashScreen(xdl::IPXdevLCore core ,xdl::XdevLOpenGLContext* openglContext, xdl::XdevLTextLayout* textLayoutSystem) :
			m_core(core),
			m_window(nullptr),
			m_openglContext(openglContext),
			m_running(xdl::xdl_true),
			m_textLayboutSystem(textLayoutSystem) 
		{
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
			thread::XdevLScopeLock lock(m_mutex);
			m_running = xdl::xdl_false;
		}

		xdl::xdl_bool SplashScreen::getRunningState() {
			thread::XdevLScopeLock lock(m_mutex);
			xdl::xdl_bool tmp;
			tmp = m_running;
			return tmp;
		}

		int SplashScreen::RunThread(thread::ThreadArgument* p_arg) {
			m_window = xdl::createModule<xdl::IPXdevLWindow>(m_core, xdl::XdevLModuleName("XdevLWindow"), xdl::XdevLID("SplashWindow"));

			xdl::IPXdevLRAI m_openGL = static_cast<xdl::IPXdevLRAI>(m_core->createModule(xdl::XdevLModuleName("XdevLOpenGLContext"), xdl::XdevLID("SplashScreenContext")));
			m_openglContext->create(m_window);
			m_openglContext->makeCurrent(m_window);

//			soan::XdevLFontImpl* fontEngine = new soan::XdevLFontImpl(m_window->getWidth(), m_window->getHeight(), m_openGL);
//			soan::TextureServer::Inst()->setResourcePathPrefix("./");
//
//			fontEngine->setCreateTextureCallback(createTextureFromFile);
//			fontEngine->importFromFontFile("resources/fonts/default_info.txt");
//
//			soan::XdevLTextLayout* textLayouter = new soan::XdevLTextLayoutImpl(m_window, m_openGL);
//			textLayouter->init(fontEngine);


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
//				xdl::XdevLOpenGLContextScope scope(m_openGL, m_window);

				
				if(!getRunningState()) {
					break;
				}

				glClearColor(1.0f, 0.0, 0.0, 1.0);
				glClear(GL_COLOR_BUFFER_BIT);


				m_openGL->setActiveShaderProgram(sp);
				m_openGL->setActiveVertexArray(va);

				m_openGL->drawVertexArray(xdl::XDEVL_PRIMITIVE_TRIANGLES, 6);

				
				std::wstring maxVertices = L"Singularity";
				m_textLayboutSystem->setScale(1.0f);
				m_textLayboutSystem->setColor(255, 255, 255, 255);
				m_textLayboutSystem->addDynamicText(maxVertices.c_str(), -0.10, 0.0);

				m_textLayboutSystem->render();

				m_openglContext->swapBuffers();

				xdl::sleep(0.100);
			}

			xdl::destroyModule(m_core, m_window);
			return 0;
		}

	}

}
