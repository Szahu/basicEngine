// -------- Entry Point---------------------
#include <Engine/Core/EntryPoint.h>      
// -----------------------------------------
#include <Engine.h>
#include "imgui/imgui.h"

#include "Engine/Platform/OpenGl/OpenGLShader.h"

#include "Sandbox2D.h"
#include "Sandbox3D.h"


class Sandbox : public Engine::Application
{
public:
	Sandbox()
	{
		
		PushLayer(new Sandbox2D());
		//PushLayer(new Sandbox3D());
	}
	~Sandbox()
	{

	}
};

Engine::Application* Engine::CreateApplication()
{
	return new Sandbox();
}