#pragma once

#include "Engine/Toolbox/PerspectiveCameraController.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Entity/Entity.h"

#include "Engine/Core/Timestep.h"
#include "Engine/Toolbox/MousePicker.h"
#include "Engine/Events/MouseEvent.h"

#include "Engine/Renderer/Skybox.h"

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


		void AddLight(const PointLight& light) { m_Lights.push_back(light); }
		const std::vector<PointLight>& GetLights() { return m_Lights; }
		ShaderLibrary& GetShaderLibrary() { return m_ShaderLibrary; }
		PerspectiveCameraController& GetCamera() { return m_Camera; }

		const Skybox& GetSkybox() { return m_Skybox; }

	private:
		bool OnMouseClick(MouseButtonPressedEvent& e);

	private:
		Entity* m_ActiveEntity = nullptr;
		Ref<FrameBuffer> m_FrameBuffer;
		PerspectiveCameraController m_Camera;
		ShaderLibrary m_ShaderLibrary;
		std::vector<PointLight> m_Lights;
		std::unordered_map<std::string, Entity> m_Entities;
		MousePicker m_MousePicker;

		glm::vec3 lampPos = { 0.0f, 0.0f, 15.0f };
		Skybox m_Skybox;

		static Scene* s_Instance;
	};
}