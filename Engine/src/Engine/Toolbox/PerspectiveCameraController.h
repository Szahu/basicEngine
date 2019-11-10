#pragma once

#include "Engine/Toolbox/PerspectiveCamera.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Engine/Core/Timestep.h"
#include "Engine/Events/MouseEvent.h"
#include "Engine/Events/ApplicationEvent.h"

#include "Engine/Renderer/FrameBuffer.h"

#include "Engine/GUI/ViewportWindow.h"

#include "Engine/Core/Application.h"

namespace Engine
{
	class PerspectiveCameraController
	{
	public:
		PerspectiveCameraController(float fov = 65.0f, float aspecRatio = 1.6f, ViewportWindow* window = Application::Get().GetViewportWindowPointer());
		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		inline const PerspectiveCamera& GetCamera() const { return m_Camera; }
		PerspectiveCamera& GetCamera() { return m_Camera; }

		bool OnMouseScroll(MouseScrolledEvent& e);
		bool OnMouseButton(MouseButtonPressedEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		void OnGuiWindowResize(const glm::vec2& size);

	private:
		ViewportWindow* m_ActiveWindow;
		float m_AspectRatio;
		float m_FOV;
		int m_ScreenWidth, m_ScreenHeight;
		float m_ZoomLevel = 1.0f;
		PerspectiveCamera m_Camera;

		glm::vec2 m_Angles = { 3.14f, 0.0f };
		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 2.0f };
		float m_CameraMoveSpeed = 15.0f;
		float m_MouseSpeed = 1.0f;
	};
}
