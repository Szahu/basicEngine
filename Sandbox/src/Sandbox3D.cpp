#include "EGpch.h"
#include "Sandbox3D.h"

#include "Glad/glad.h"
#include "imgui.h"

#include "GLFW/glfw3.h"

#include "Engine/Entity/Component.h"
#include "Engine/Toolbox/Samples/basicMeshes.h"



Sandbox3D::Sandbox3D()
	:Layer("Sandbox3D")
{

}

void Sandbox3D::OnAttach()
{

	EG_INFO("Sanbox3D online");


	testScene.LoadScene();

	testScene.AddEntity(Engine::Entity("Material Test"));
	testScene.GetEntity("Material Test")->AddComponent(Engine::ComponentType::Mesh);
	//testScene.GetEntity("Material Test")->GetMeshComponent()->GetMaterial().m_DiffuseTexture = Engine::Texture2D::Create("assets/textures/brickwall/diffuse.jpg");

	for (int i = 0; i < 1; i++)
	{
		testScene.AddEntity(Engine::Entity("Entity " + std::to_string(i)));
		testScene.GetEntity("Entity " + std::to_string(i))->AddComponent(Engine::ComponentType::Model);
		//testScene.GetEntity("Entity " + std::to_string(i))->GetModelComponent()->LoadModel("assets/models/nanosuit/scene.gltf");
		//testScene.GetEntity("Entity " + std::to_string(i))->GetModelComponent()->LoadModel("assets/models/stone/scene.gltf");
		//testScene.GetEntity("Entity " + std::to_string(i))->GetModelComponent()->LoadModel("assets/models/SF_FIGHTER/Scifi_Fighter.FBX");
	}
}

void Sandbox3D::OnDetach()
{

}

void Sandbox3D::OnUpdate(Engine::Timestep ts)
{
	FPS = 1.0f / ts;


	testScene.OnUpdate(ts);

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
	testScene.OnEvent(event);
}

