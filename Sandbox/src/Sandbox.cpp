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
		EG_INFO("ExampleLayer::Update");
	}

	void OnEvent(Engine::Event& event) override
	{
		EG_TRACE("{0}", event);
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