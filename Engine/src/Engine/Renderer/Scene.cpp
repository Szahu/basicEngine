#include "EGpch.h"
#include "Scene.h"

#include "imgui.h"

#include "Engine/Core/Application.h"
#include "Engine/Core/MouseButtonCodes.h"
#include "Engine/Toolbox/Samples/BasicMeshes.h"
#include "Engine/Core/Input.h"

#include "glad/glad.h"

namespace Engine
{
	
	Scene* Scene::s_Instance = nullptr;

	Scene::Scene()
		:m_MousePicker(Application::Get().GetViewportWindowPointer()), m_Camera(65.0f, 1.6f)
	{
		s_Instance = this;
		PointLight light;
		PointLight light1;
		//m_PointLights.push_back(light);
		//m_PointLights.push_back(light1);

		SpotLight slight;
		SpotLight slight1;
		m_SpotLights.push_back(slight);
		//m_SpotLights.push_back(slight1);
		
	}

	void Scene::LoadScene()
	{
		InstrumentationTimer timer("Scene::LoadScene()");

		m_ShaderLibrary.Load("assets/shaders/GuiQuad.glsl");
		m_ShaderLibrary.Load("assets/shaders/Model.glsl");
		m_ShaderLibrary.Load("assets/shaders/Material.glsl");
		m_ShaderLibrary.Load("assets/shaders/FlatColor.glsl");
		m_ShaderLibrary.Load("assets/shaders/Reflection.glsl");
		m_ShaderLibrary.Load("assets/shaders/simpleDepthShader.glsl");
		m_ShaderLibrary.Load("assets/shaders/DebugDepthQuad.glsl");
		m_ShaderLibrary.Load("assets/shaders/DisplayNormals.glsl");

		m_ShaderLibrary.Get("GuiQuad")->Bind();
		m_ShaderLibrary.Get("GuiQuad")->SetInt1("texture_sample", 0);
		m_ShaderLibrary.Get("GuiQuad")->Unbind();
		
		m_Skybox.Load("assets/textures/skyboxes/nightSky");

		Renderer::InitScene(); 

		GuiQuad = BasicMeshes::Quad();

		m_LightGuiTexture = Texture2D::Create("assets/textures/lightBulb.png", true);

	}

	void Scene::OnUpdate(Timestep ts)
	{
		PROFILE_FUNCTION();

		Renderer::BeginScene();

		m_Camera.OnUpdate(ts);
		m_MousePicker.OnUpdate(m_Camera.GetCamera().GetProjectionMatrix(), m_Camera.GetCamera().GetViewMatrix());


		//Actual Rendering
		Application::Get().GetViewportWindowPointer()->GetFrameBuffer()->Bind();
		Engine::RenderCommand::Clear();
		Engine::RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 0.0f });
		glStencilMask(0x00);

		RenderScene();
		//eyyyy
		DrawGui();	
		
		Application::Get().GetViewportWindowPointer()->GetFrameBuffer()->Unbind();
		Engine::RenderCommand::Clear();
		glStencilMask(0x00);

		Renderer::EndScene();
	}

	void Scene::RenderScene()
	{
		m_Skybox.Draw(m_Camera.GetCamera());

		for (auto& ent : m_Entities)
		{
			ent.second.OnUpdate();
			ent.second.CheckForIntersection(&m_MousePicker);
			ent.second.CheckIfActive(m_ActiveEntity);
		}
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
		for (int i = 0;i < m_PointLights.size();i ++)
		{
			if (m_ActivePointLight == i && m_ActiveLightType == LightType::Point) node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_Selected;
			else node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
			bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, ("PointLight" + std::to_string(i)).c_str(), i);
			if (ImGui::IsItemClicked())
			{
				node_clicked = i;
				m_ActivePointLight = i;
				m_ActiveLightType = LightType::Point;
			}
				
			if (node_open)
			{
				ImGui::Text("Blah blah\nBlah Blah");
				
				ImGui::TreePop();
			}

		}

		for (int i = 0; i < m_SpotLights.size(); i++)
		{
			if (m_ActiveSpotLight == i && m_ActiveLightType == LightType::Spot) node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_Selected;
			else node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
			bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, ("SpotLight" + std::to_string(i)).c_str(), i);
			if (ImGui::IsItemClicked())
			{
				node_clicked = i;
				m_ActiveSpotLight = i;
				m_ActiveLightType = LightType::Spot;
			}

			if (node_open)
			{
				ImGui::Text("Blah blah\nBlah Blah");

				ImGui::TreePop();
			}

		}

		if(m_PointLights.size() > 0 && m_ActivePointLight < m_PointLights.size() && m_ActivePointLight >= 0 && m_ActiveLightType == LightType::Point)
			m_PointLights[m_ActivePointLight].OnImGuiRender();


		if (m_SpotLights.size() > 0 && m_ActiveSpotLight < m_SpotLights.size() && m_ActiveSpotLight >= 0 && m_ActiveLightType == LightType::Spot)
			m_SpotLights[m_ActiveSpotLight].OnImGuiRender();

		if (ImGui::Button("Add PointLight!"))
		{
			if (m_PointLights.size() < Renderer::MAX_NUMBER_OF_POINTLIGHTS)
			{
				PointLight light;
				m_PointLights.push_back(light);
			}
			else
				EG_CORE_WARN("Reached max amount of Pointlights!");
		}

		if (ImGui::Button("Add SpotLight!"))
		{
			if (m_SpotLights.size() < Renderer::MAX_NUMBER_OF_SPOTLIGHTS)
			{
				SpotLight light;
				m_SpotLights.push_back(light);
			}
			else
				EG_CORE_WARN("Reached max amount of Spotlights!");
		}

		//ImGui::DragFloat3("Spot pos", &m_SpotLights[1].GetLightData().Position.x, 0.05f);

		ImGui::End();
	}

	void Scene::DrawGui()
	{
		glDisable(GL_STENCIL_TEST);
		GuiQuad->Bind();
		m_ShaderLibrary.Get("GuiQuad")->Bind();
		m_LightGuiTexture->Bind();

		glm::mat4 ViewMatrix = m_Camera.GetCamera().GetViewMatrix();
		glm::vec3 camRight = { ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0] };
		glm::vec3 camUp = { ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1] };

		m_ShaderLibrary.Get("GuiQuad")->SetFloat3("u_CameraRight", camRight);
		m_ShaderLibrary.Get("GuiQuad")->SetFloat3("u_CameraUp", camUp);

		for (int i = 0; i < m_PointLights.size(); i++)
		{
			glm::vec4 pos = Scene::GetActiveScene().GetPointLights()[i].GetLightData().Position;
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(pos));
			m_ShaderLibrary.Get("GuiQuad")->SetMat4("u_Transform", transform);
			RenderCommand::DrawIndexed(GuiQuad);
		}
		GuiQuad->Unbind();
		m_ShaderLibrary.Get("GuiQuad")->Unbind();
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);
	}


}
