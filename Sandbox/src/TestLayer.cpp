#include "TestLayer.h"

#include "GLFW/glfw3.h"
#include "Engine/Core/Keycodes.h"

using namespace Engine;

void TestLayer::OnAttach()
{
	for (int i = 0; i < 16; i++)
	{
		if (glfwGetJoystickName(i))
		{
			ID = i;
		}
	}
}

void TestLayer::OnDetach()
{
}

void TestLayer::OnUpdate(Engine::Timestep ts)
{
	
	Input::GetGamepadTriggersPos();

}

void TestLayer::OnImGuiRender()
{
}

void TestLayer::OnEvent(Engine::Event& event)
{
}
