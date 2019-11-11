#pragma once

#include "Engine/Toolbox/PerspectiveCameraController.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Entity/Entity.h"

#include "Engine/Core/Timestep.h"

#include "Engine/Toolbox/MousePicker.h"

#include "Engine/Events/MouseEvent.h"

namespace Engine
{

	class Scene
	{
	public:
		Scene();

		void OnUpdate(Timestep ts);
		void OnImGuiRender();
		void OnEvent(Event& e);

		void SetSceneData(const Ref<FrameBuffer>& frameBuffer, PerspectiveCameraController* camera, const std::vector<PointLight*>& lights, ShaderLibrary* library);

		void AddEntity(Entity entity);

	private:
		bool OnMouseClick(MouseButtonPressedEvent& e);

	private:
		Entity* m_ActiveEntity = nullptr;
		Ref<FrameBuffer> m_FrameBuffer;
		PerspectiveCameraController* m_Camera;
		ShaderLibrary* m_ShaderLibrary;
		std::vector<PointLight*> m_Lights;
		std::unordered_map<std::string, Entity> m_Entities;
		MousePicker m_MousePicker;

	};
}