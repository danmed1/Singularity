#ifndef SPLASH_SCREEN_H
#define SPLASH_SCREEN_H

#include <XdevL.h>
#include <XdevLOpenGL/XdevLOpenGL.h>
#include <XdevLMutex.h>

namespace soan {

	namespace utils {
		
		class SplashScreen : public xdl::XdevLListener {
			public:
				SplashScreen(xdl::IPXdevLCore core, xdl::XdevLOpenGL330* opengl);
				~SplashScreen();
				xdl::XdevLID* getID();
				xdl::xdl_int notify(xdl::XdevLEvent& event);
				void show();
				void hide();
		private:
				xdl::xdl_bool getRunningState();
		private:
				xdl::IPXdevLCore 			m_core;
				xdl::XdevLOpenGL330*	m_openGL;
				xdl::xdl_bool 				m_running;
				thread::Mutex					m_mutex;
		};

	}

}

#endif // SPLASH_SCREEN_H
