#pragma once

#include "Renderer/OrtographicCamera.h"
#include "Events/ApplicationEvent.h"
#include "Events/MouseEvent.h"
#include "Engine/Core/Timestep.h"

namespace Engine
{
	class OrtographicCameraController
	{
	public:
		OrtographicCameraController(float aspectRatio, bool rotation = false); 

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		inline const float GetZoomLevel() { return m_ZoomLevel; }
		inline const void SetZoomLevel(float level) { m_ZoomLevel = level; }

		inline const OrtographicCamera& GetCamera() const { return m_Camera; }
		OrtographicCamera& GetCamera() { return m_Camera; }

	private:
		bool onMouseScrolled(MouseScrolledEvent& e);
		bool onWindowResize(WindowResizeEvent& e);

	private:
		bool m_Rotation;
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrtographicCamera m_Camera;

		float m_CameraRotation = 0.0f;
		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraTranslationSpeed = 5.0f;
		float m_CameraRotationSpeed = 180.0f;
	};
}