#include "SplashScreen.h"
#include <XdevLWindow/XdevLWindow.h>

namespace soan {

	namespace utils {

		SplashScreen::SplashScreen(xdl::IPXdevLCore core, xdl::XdevLOpenGL330* opengl) : 	m_core(core),
			m_openGL(opengl),
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
			xdl::IPXdevLWindow window = xdl::createModule<xdl::IPXdevLWindow>(m_core, "XdevLWindow", "SplashWindow");

			while(true) {
				if(!getRunningState()) {
					break;
				}
			
			}

			xdl::destroyModule(m_core, window);
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



	}

}
