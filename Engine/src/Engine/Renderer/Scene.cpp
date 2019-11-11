#include "EGpch.h"
#include "Scene.h"

#include "imgui.h"

#include "Engine/Core/Application.h"
#include "Engine/Core/MouseButtonCodes.h"

#include "Engine/Core/Input.h"

namespace Engine
{

	Scene::Scene()
		:m_ShaderLibrary(nullptr), m_MousePicker(Application::Get().GetViewportWindowPointer())
	{

	}

	void Scene::OnUpdate(Timestep ts)
	{

		Renderer::BeginScene(m_FrameBuffer, m_Camera->GetCamera(), m_Lights, m_ShaderLibrary);

		m_Camera->OnUpdate(ts);
		m_MousePicker.OnUpdate(m_Camera->GetCamera().GetProjectionMatrix(), m_Camera->GetCamera().GetViewMatrix());

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

		if (ImGui::IsMouseClicked(1) && ImGui::IsWindowHovered())
		{
			ImGui::OpenPopup("Test_popup"); 
		}

		if(ImGui::BeginPopup("Test_popup"))
		{
			if(ImGui::BeginPopup("EnterText"))
			{
				ImGui::SameLine();
				static char text[16];
				if (ImGui::InputText("Name:", &text[0], IM_ARRAYSIZE(text), ImGuiInputTextFlags_EnterReturnsTrue))
				{
					if (text[0] != 0)
					{
						Entity new_Entity(text);
						AddEntity(new_Entity);
						text[0] = 0;
					}
					ImGui::CloseCurrentPopup();
				}
				ImGui::SameLine();

				if (ImGui::Button("Enter")) 
				{ 
					if (text[0] != 0)
					{
						Entity new_Entity(text);
						AddEntity(new_Entity);
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
			

		for (auto& en : m_Entities)
		{	
			if (m_ActiveEntity == &en.second)
			{
				ImGui::Button((en.second.GetName() + " (Active)").c_str());
			}

			else
			{
				if (ImGui::Button(en.second.GetName().c_str()))
				{
					m_ActiveEntity = &en.second;
				}
			}
			
		}
		ImGui::End();

		if(m_ActiveEntity != nullptr) m_ActiveEntity->OnImGuiRender();
	}

	void Scene::OnEvent(Event& e)
	{
		m_Camera->OnEvent(e);
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseButtonPressedEvent>(EG_BIND_EVENT_FN(Scene::OnMouseClick));
	}

	void Scene::SetSceneData(const Ref<FrameBuffer>& frameBuffer, PerspectiveCameraController* camera, const std::vector<PointLight*>& lights, ShaderLibrary* library)
	{
		m_FrameBuffer = frameBuffer;
		m_Camera = camera;
		m_ShaderLibrary = library;
		m_Lights = lights;
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
		//m_Entities[entity.GetName()] = entity;
		m_Entities.emplace(std::make_pair(entity.GetName(), entity));
		//m_Entities.insert(std::make_pair(entity.GetName(), entity));
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

}
