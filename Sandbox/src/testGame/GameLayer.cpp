#include "GameLayer.h"

using namespace Engine;;

GameLayer::GameLayer()
	: Layer("GameLayer")
{
	auto& window = Application::Get().GetWindow();

	CreateCamera(window.GetWidth(), window.GetHeight());

	Random::Init();
}

void GameLayer::OnAttach()
{

}

void GameLayer::OnDetach()
{

}

void GameLayer::OnUpdate(Engine::Timestep ts)
{

}

void GameLayer::OnImGuiRender()
{

}

void GameLayer::OnEvent(Engine::Event& event)
{

}

void GameLayer::CreateCamera(uint32_t width, uint32_t height)
{
	float aspectRatio = (float)width / (float)height;

	float camWidth = 8.0f;
	float bottom = -camWidth;
	float top = camWidth;
	float left = bottom * aspectRatio;
	float right = top * aspectRatio;

	m_Camera = CreateScope<OrtographicCamera>(left, right, bottom, top);
}
