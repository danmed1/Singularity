#include "Application.h"

Application* Application::ms_instance = 0;

Application::Application()
{
}

Application::~Application()
{
}

Application* Application::Instance()
{
	if (ms_instance == 0) {
		ms_instance = new Application();
	}
	return ms_instance;
}

void Application::Release()
{
	if (ms_instance) {
		delete ms_instance;
	}
	ms_instance = 0;
}

