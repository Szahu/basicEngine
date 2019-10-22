#include <Engine.h>

class ExampleLayer : public Engine::Layer
{
public:
	ExampleLayer()
		: Layer("example")
	{

	}

	void OnUpdate() override
	{
		if (Engine::Input::IsKeyPressed(EG_KEY_TAB))
			EG_INFO("Tab key has been just presed!");
	}

	void OnEvent(Engine::Event& event) override
	{

	}

};

class Sandbox : public Engine::Application
{
public:
	Sandbox()
	{
		EG_INFO("Sandbox app Online");

		PushLayer(new ExampleLayer());
	}
	~Sandbox()
	{

	}
};

Engine::Application* Engine::CreateApplication()
{
	return new Sandbox();
}