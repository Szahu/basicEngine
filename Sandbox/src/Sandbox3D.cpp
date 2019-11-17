#include "EGpch.h"
#include "Sandbox3D.h"

#include "Glad/glad.h"
#include "imgui.h"

#include "GLFW/glfw3.h"

#include "Engine/Entity/Component.h"
#include "Engine/Toolbox/Samples/basicMeshes.h"

#include "stb_image.h"

Sandbox3D::Sandbox3D()
	:Layer("Sandbox3D"), m_CameraController(65.0f, 1280.0f / 720.0f, &m_Window)
{

}

void Sandbox3D::OnAttach()
{
	

	EG_INFO("Sanbox3D online");

	//m_Lights.push_back(&m_Light);
	m_Lights.push_back(&m_Light1);

	testScene.LoadScene();

	for (int i = 0; i < 1; i++)
	{
		testScene.AddEntity(Engine::Entity("Entity " + std::to_string(i)));
		testScene.GetEntity("Entity " + std::to_string(i))->AddComponent(Engine::ComponentType::Model);
		testScene.GetEntity("Entity " + std::to_string(i))->GetModelComponent()->LoadModel("assets/models/SF_FIGHTER/Scifi_Fighter.fbx");
		//testScene.GetEntity("Entity " + std::to_string(i))->GetModelComponent()->LoadModel("assets/models/nanosuit/scene.gltf");
		//testScene.GetEntity("Entity " + std::to_string(i))->GetModelComponent()->LoadModel("assets/models/the Shrek/scene.gltf");
	}

}

void Sandbox3D::OnDetach()
{

}

void Sandbox3D::OnUpdate(Engine::Timestep ts)
{
	FPS = 1.0f / ts;


	testScene.OnUpdate(ts);


	/* Playground:
	Engine::Application::Get().GetViewportWindowPointer()->GetFrameBuffer()->Bind();
	Engine::RenderCommand::Clear();
	Engine::RenderCommand::SetClearColor({ 0.53f, 0.81f, 0.98f, 1.0f });
	glStencilMask(0xFF);
	
	
	Engine::Application::Get().GetViewportWindowPointer()->GetFrameBuffer()->Unbind();
	Engine::RenderCommand::Clear();
	glStencilMask(0x00);
	*/

}

void Sandbox3D::OnImGuiRender()
{

	Engine::Gui::InitDocspace();

	m_Window.OnImGuiRender();

	bool show = true;
	ImGui::ShowDemoWindow(&show);
	ImGui::Begin("Inspector");
	testScene.OnImGuiRender();
	ImGui::Text("FPS: %i", FPS);
	ImGui::End();

}

void Sandbox3D::OnEvent(Engine::Event& event)
{
	m_CameraController.OnEvent(event);
	testScene.OnEvent(event);
}

