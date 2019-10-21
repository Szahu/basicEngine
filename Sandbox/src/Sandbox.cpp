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
		PushOverlay(new Engine::ImGuiLayer());
	}
	~Sandbox()
	{

	}
};

Engine::Application* Engine::CreateApplication()
{
	return new Sandbox();
}