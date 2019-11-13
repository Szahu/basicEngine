#include "EGpch.h"
#include "Sandbox3D.h"

#include "Glad/glad.h"
#include "imgui.h"

#include "GLFW/glfw3.h"

#include "Engine/Entity/Component.h"
#include "Engine/Toolbox/Samples/basicMeshes.h"

Sandbox3D::Sandbox3D()
	:Layer("Sandbox3D"), m_CameraController(65.0f, 1280.0f / 720.0f, &m_Window)
{

}

void Sandbox3D::OnAttach()
{
	

	EG_INFO("Sanbox3D online");

	m_ShaderLibrary.Load("assets/shaders/Material.glsl");
	m_ShaderLibrary.Load("assets/shaders/MaterialTexture.glsl");
	m_ShaderLibrary.Load("assets/shaders/Model.glsl");
	m_ShaderLibrary.Load("assets/shaders/FlatColor.glsl");

	//m_Lights.push_back(&m_Light);
	m_Lights.push_back(&m_Light1);

	testScene.LoadScene();

	for (int i = 0; i < 1; i++)
	{
		testScene.AddEntity(Engine::Entity("Entity " + std::to_string(i)));
		testScene.GetEntity("Entity " + std::to_string(i))->AddComponent(Engine::ComponentType::Model);
		testScene.GetEntity("Entity " + std::to_string(i))->GetModelComponent()->LoadModel("assets/models/sf_fighter/sCIfI_fIGHTER.FBX");
	}
}

void Sandbox3D::OnDetach()
{

}

void Sandbox3D::OnUpdate(Engine::Timestep ts)
{
	

	testScene.OnUpdate(ts);
	FPS = 1.0f / ts;
	glStencilMask(0x00);


	glm::mat4 lampTransform1 = glm::translate(glm::mat4(1.0f), m_LampPosition1) * glm::scale(glm::mat4(1.0f), { 0.5f, 0.5f, 0.5 });
	m_Light1.SetPosition(glm::vec3(lampTransform1[3][0], lampTransform1[3][1], lampTransform1[3][2]));

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
	//ImGui::DragFloat3("LightPos", &m_LampPosition1.x, 0.3f);
	ImGui::End();
}

void Sandbox3D::OnEvent(Engine::Event& event)
{
	m_CameraController.OnEvent(event);
	testScene.OnEvent(event);
}

