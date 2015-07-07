#ifndef APPLICATION
#define APPLICATION

class Application
{
	static Application* ms_instance;

private:
	Application(const Application& rhs);
	Application& operator=(const Application& rhs);

public:
	static Application* Instance();
	static void Release();

private:
	Application();
	virtual ~Application();

};

#endif // APPLICATION
