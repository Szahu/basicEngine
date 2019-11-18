#include "EGpch.h"
#include "Scene.h"

#include "imgui.h"

#include "Engine/Core/Application.h"
#include "Engine/Core/MouseButtonCodes.h"

#include "Engine/Core/Input.h"

namespace Engine
{

	Scene* Scene::s_Instance = nullptr;

	Scene::Scene()
		:m_MousePicker(Application::Get().GetViewportWindowPointer()), m_Camera(65.0f, 1.6f)
	{
		s_Instance = this;
		PointLight light;
		PointLight light1;
		m_Lights.push_back(light);
		m_Lights.push_back(light1);
		
	}

	void Scene::LoadScene()
	{
		m_ShaderLibrary.Load("assets/shaders/Material.glsl");
		m_ShaderLibrary.Load("assets/shaders/MaterialTexture.glsl");
		m_ShaderLibrary.Load("assets/shaders/Model.glsl");
		m_ShaderLibrary.Load("assets/shaders/FlatColor.glsl");
		m_ShaderLibrary.Load("assets/shaders/Reflection.glsl");

		m_FrameBuffer = Application::Get().GetViewportWindowPointer()->GetFrameBuffer();
		
		m_Skybox.Load("assets/textures/skyboxes/waterAndSky");

		Renderer::InitScene(); 
	}

	void Scene::OnUpdate(Timestep ts)
	{
		Renderer::BeginScene();

		m_Camera.OnUpdate(ts);
		m_MousePicker.OnUpdate(m_Camera.GetCamera().GetProjectionMatrix(), m_Camera.GetCamera().GetViewMatrix());

		m_Skybox.Draw(m_Camera.GetCamera());

		for (auto& ent : m_Entities)
		{
			ent.second.OnUpdate();
			ent.second.CheckForIntersection(&m_MousePicker);
			ent.second.CheckIfActive(m_ActiveEntity);
		}

		Renderer::EndScene();
	}

	void Scene::OnImGuiRender()
	{

		ImGui::Begin("Entities in the scene");

		AddingEntityPopUp();
		EntityInspectorWindowContent();

		EnvironmentWindow();

		ImGui::End();

		if(m_ActiveEntity != nullptr) m_ActiveEntity->OnImGuiRender();
	}

	void Scene::OnEvent(Event& e)
	{
		m_Camera.OnEvent(e);
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseButtonPressedEvent>(EG_BIND_EVENT_FN(Scene::OnMouseClick));
	}

	void Scene::AddEntity(Entity entity)
	{
		for (auto& em : m_Entities)
		{
			if (entity.GetName() == em.first)
			{
				EG_CORE_WARN("There already is an Entity with this name!");
				return;
			}
		}
		m_Entities.emplace(std::make_pair(entity.GetName(), entity));
		m_ActiveEntity = &m_Entities[entity.GetName()];
	}


	bool Scene::OnMouseClick(MouseButtonPressedEvent& e)
	{
		if (e.GetMouseButton() == EG_MOUSE_BUTTON_1)
		{
			for (auto& ent : m_Entities)
			{
				if (ent.second.CheckForIntersection(&m_MousePicker))
				{
					m_ActiveEntity = &ent.second;
					return false;
				}
			}
			return false;
		}

		return false;

	}

	void Scene::EntityInspectorWindowContent()
	{
		int i = 0;
		int node_clicked = -1;
		ImGuiTreeNodeFlags node_flags;
		for (auto& en : m_Entities)
		{
			if (m_ActiveEntity == &en.second) node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_Selected;
			else node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
			i++;
			bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, en.first.c_str(), i);
			if (ImGui::IsItemClicked())
			{
				node_clicked = i;
				m_ActiveEntity = &en.second;
			}
				
			if (node_open)
			{
				ImGui::Text("Blah blah\nBlah Blah");
				
				ImGui::TreePop();
			}

		}
	}

	void Scene::AddingEntityPopUp()
	{
		if (ImGui::IsMouseClicked(1) && ImGui::IsWindowHovered())
		{
			ImGui::OpenPopup("Test_popup");
		}

		if (ImGui::BeginPopup("Test_popup"))
		{
			if (ImGui::BeginPopup("EnterText"))
			{
				static char text[16];
				if (ImGui::InputText("Name:", &text[0], IM_ARRAYSIZE(text), ImGuiInputTextFlags_EnterReturnsTrue))
				{
					if (text[0] != 0)
					{
						AddEntity(Entity(text));
						text[0] = 0;
					}
				}

				ImGui::SameLine();

				if (ImGui::Button("Enter"))
				{
					if (text[0] != 0)
					{
						AddEntity(Entity(text));
						text[0] = 0;
					}
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}

			if (ImGui::Button("Add new Entity!"))
			{
				ImGui::OpenPopup("EnterText");
			}

			ImGui::EndPopup();
		}
	}

	void Scene::EnvironmentWindow()
	{
		ImGui::Begin("Environment");

		int node_clicked = -1;
		ImGuiTreeNodeFlags node_flags;
		for (int i = 0;i < m_Lights.size();i ++)
		{
			if (m_ActiveLight == i) node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_Selected;
			else node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
			bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, ("PointLight" + std::to_string(i)).c_str(), i);
			if (ImGui::IsItemClicked())
			{
				node_clicked = i;
				m_ActiveLight = i;
			}
				
			if (node_open)
			{
				ImGui::Text("Blah blah\nBlah Blah");
				
				ImGui::TreePop();
			}

		}

		if(m_Lights.size() > 0 && m_ActiveLight < m_Lights.size() && m_ActiveLight >= 0)
			m_Lights[m_ActiveLight].OnImGuiRender();

		ImGui::End();
	}

}
