#include <Engine.h>

class Sandbox : public Engine::Application
{
public:
	Sandbox()
	{
		EG_INFO("Welcome to sandbox app");
	}
	~Sandbox()
	{

	}
};

Engine::Application* Engine::CreateApplication()
{
	return new Sandbox();
}