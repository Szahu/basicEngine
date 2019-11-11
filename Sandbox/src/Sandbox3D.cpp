#include "EGpch.h"
#include "Sandbox3D.h"

#include "Glad/glad.h"
#include "imgui.h"

#include "GLFW/glfw3.h"

#include "Engine/Entity/Components/MeshComponent.h"
#include "Engine/Entity/Components/TransformComponent.h"

Sandbox3D::Sandbox3D()
	:Layer("Sandbox3D"), m_CameraController(65.0f, 1280.0f / 720.0f, &m_Window)
{
}

void Sandbox3D::OnAttach()
{
	Engine::Application::Get().SetEditorCameraPointer(&m_CameraController.GetCamera());

	EG_INFO("Sanbox3D online");

	m_ShaderLibrary.Load("assets/shaders/Material.glsl");
	m_ShaderLibrary.Load("assets/shaders/FlatColor.glsl");

	m_Lights.push_back(&m_Light);
	m_Lights.push_back(&m_Light1);

	testScene.SetSceneData(m_Window.GetFrameBuffer(), &m_CameraController, m_Lights, &m_ShaderLibrary);
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

	/*
	testEntity.OnUpdate();

	m_CameraController.OnUpdate(ts);
	m_MousePicker.OnUpdate(m_CameraController.GetCamera().GetProjectionMatrix(), m_CameraController.GetCamera().GetViewMatrix());	

	const glm::vec3 test = { 0.0f, 0.0f, 0.0f };
	float distance = 0.0f;
	bool isInterSecting = glm::intersectRaySphere(m_CameraController.GetCamera().GetPosition(), m_MousePicker.GetCurrentRay(), m_LampPosition1, 1.0f, distance);

	Engine::Renderer::BeginScene(m_Window.GetFrameBuffer(), m_CameraController.GetCamera(), m_Lights, &m_ShaderLibrary);
	
	glStencilMask(0x00);

	m_ShaderLibrary.Get("FlatColor")->Bind();
	m_ShaderLibrary.Get("FlatColor")->SetFloat4("u_Color", { 1.0f, 1.0f, 1.0f, 1.0f });
	Engine::Renderer::Submit(m_Model, testEntity.GetTransformComponent()->GetTransform());

	glm::mat4 lampTransform1 = glm::translate(glm::mat4(1.0f), m_LampPosition1) * glm::scale(glm::mat4(1.0f), { 0.5f, 0.5f, 0.5 });
	m_Light1.SetPosition(glm::vec3(lampTransform1[3][0], lampTransform1[3][1], lampTransform1[3][2]));

	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);

	Engine::Renderer::Submit(m_LampVertexArray, lampTransform1, m_ShaderLibrary.Get("FlatColor"));

	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);
	//glDisable(GL_DEPTH_TEST);

	m_ShaderLibrary.Get("FlatColor")->SetFloat4("u_Color", { 0.7f, 2.0f, 0.0f, 1.0f });
	Engine::Renderer::Submit(m_LampVertexArray, glm::scale(lampTransform1, glm::vec3(1.2f, 1.2f, 1.2f)), m_ShaderLibrary.Get("FlatColor"));

	glStencilMask(0x00);
	//glEnable(GL_DEPTH_TEST);

	
	m_ShaderLibrary.Get("FlatColor")->SetFloat4("u_Color", { 1.0f, 1.0f, 1.0f, 1.0f });
	glm::mat4 lampTransform = glm::translate(glm::mat4(1.0f), glm::vec3(m_LampPosition.x, m_LampPosition.y + sin(glfwGetTime()) * 5, m_LampPosition.z)) * glm::scale(glm::mat4(1.0f), { 0.5f, 0.5f, 0.5 });
	m_Light.SetPosition(glm::vec3(lampTransform[3][0], lampTransform[3][1], lampTransform[3][2]));
	//Engine::Renderer::Submit(testEntity.GetMeshComponent()->GetVertexArray(), testEntity.GetTransformComponent()->GetTransform(), m_ShaderLibrary.Get("FlatColor"));


	Engine::Renderer::EndScene();
	*/
}

void Sandbox3D::OnImGuiRender()
{
	Engine::Gui::InitDocspace();

	m_Window.OnImGuiRender();

	bool show = true;
	ImGui::ShowDemoWindow(&show);
	ImGui::Begin("Inspector");
	static char text[16];
	if (ImGui::InputText("Name:", &text[0], IM_ARRAYSIZE(text), ImGuiInputTextFlags_EnterReturnsTrue))
	{
		if (text[0] != 0)
		{
			EG_CORE_INFO(text);
			text[0] = 0;
		}
	}

	testScene.OnImGuiRender();
	ImGui::Text("FPS: %i", FPS);
	ImGui::DragFloat3("LightPos", &m_LampPosition1.x, 0.3f);
	ImGui::End();
}

void Sandbox3D::OnEvent(Engine::Event& event)
{
	m_CameraController.OnEvent(event);
	testScene.OnEvent(event);
}

