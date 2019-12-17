#pragma once

#include "Engine/Toolbox/PerspectiveCameraController.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Entity/Entity.h"

#include "Engine/Core/Timestep.h"
#include "Engine/Toolbox/MousePicker.h"
#include "Engine/Events/MouseEvent.h"

#include "Engine/Renderer/Skybox.h"

//Temporary
#include <windows.h>
#include <Commdlg.h>

namespace Engine
{

	class Scene
	{
	public:
		Scene();

		void LoadScene();

		static Scene& GetActiveScene() { return *s_Instance; }

		void OnUpdate(Timestep ts);
		void OnImGuiRender();
		void OnEvent(Event& e);

		void AddEntity(Entity entity);
		Entity* GetEntity(const std::string& name) { return &m_Entities[name]; }


		void AddPointLight(const PointLight& light) { m_PointLights.push_back(light); }
		std::vector<PointLight>& GetPointLights() { return m_PointLights; }

		void AddSpotLight(const SpotLight& light) { m_SpotLights.push_back(light); }
		std::vector<SpotLight>& GetSpotLights() { return m_SpotLights; }

		ShaderLibrary& GetShaderLibrary() { return m_ShaderLibrary; }
		PerspectiveCameraController& GetCamera() { return m_Camera; }

		const Skybox& GetSkybox() { return m_Skybox; }

		void RenderScene();

	private:
		bool OnMouseClick(MouseButtonPressedEvent& e);

		void EntityInspectorWindowContent();
		void AddingEntityPopUp();
		void EnvironmentWindow();

		void DrawGui();

	private:
		Entity* m_ActiveEntity = nullptr;
		unsigned int m_ActivePointLight = 0;
		unsigned int m_ActiveSpotLight = 0;
		LightType m_ActiveLightType = LightType::None;
		PerspectiveCameraController m_Camera;
		ShaderLibrary m_ShaderLibrary;
		std::vector<PointLight> m_PointLights;
		std::vector<SpotLight> m_SpotLights;
		std::unordered_map<std::string, Entity> m_Entities;
		MousePicker m_MousePicker;
		Ref<VertexArray> GuiQuad;
		Skybox m_Skybox;
		Ref<Texture2D> m_LightGuiTexture;
		static Scene* s_Instance;

	};
}