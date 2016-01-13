#ifndef SPLASH_SCREEN_H
#define SPLASH_SCREEN_H

#include <XdevL.h>
#include <XdevLWindow/XdevLWindow.h>
#include <XdevLOpenGLContext/XdevLOpenGLContext.h>

#include <GL/glew.h>
#include <XdevLRAI/XdevLRAI.h>
#include <XdevLFont/XdevLFont.h>
#include <XdevLFont/XdevLFontSystem.h>
#include <XdevLFont/XdevLTextLayout.h>

#include <XdevLThread.h>
#include <XdevLMutex.h>


namespace soan {

	namespace utils {
		
		class SplashScreen : public xdl::XdevLListener, public thread::Thread {
			public:
				SplashScreen(xdl::IPXdevLCore core, xdl::IPXdevLOpenGLContext openglContext, xdl::IPXdevLTextLayout textLayoutSystem);
				~SplashScreen();
				xdl::XdevLID* getID();
				xdl::xdl_int notify(xdl::XdevLEvent& event);
				void show();
				void hide();
		private:
				int RunThread(thread::ThreadArgument* p_arg) final;
				xdl::xdl_bool getRunningState();
		private:
				xdl::IPXdevLCore 					m_core;
				xdl::IPXdevLWindow				m_window;
				xdl::IPXdevLOpenGLContext	m_openglContext;
				xdl::IPXdevLRAI						m_openGL;
				xdl::IPXdevLTextLayout 		m_textLayboutSystem;
				xdl::xdl_bool 						m_running;
				thread::Mutex							m_mutex;
		};

	}

}

#endif // SPLASH_SCREEN_H
