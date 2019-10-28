#include "EGpch.h"
#include "OrtographicCameraController.h"

#include "Engine/Core/Application.h"

#include "glm/gtc/matrix_transform.hpp"
#include "Engine/Core/Input.h"
#include "Engine/Core/Keycodes.h"

namespace Engine
{
	Engine::OrtographicCameraController::OrtographicCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio),
		 m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation)
	{

	}

	void OrtographicCameraController::OnUpdate(Timestep ts)
	{

		if (Engine::Input::IsKeyPressed(EG_KEY_W))
			m_CameraPosition.y += m_CameraTranslationSpeed * ts;

		if (Engine::Input::IsKeyPressed(EG_KEY_S))
			m_CameraPosition.y -= m_CameraTranslationSpeed * ts;

		if (Engine::Input::IsKeyPressed(EG_KEY_A))
			m_CameraPosition.x -= m_CameraTranslationSpeed * ts;

		if (Engine::Input::IsKeyPressed(EG_KEY_D))
			m_CameraPosition.x += m_CameraTranslationSpeed * ts;

		if (m_Rotation)
		{
			if (Engine::Input::IsKeyPressed(EG_KEY_Q))
				m_CameraRotation += m_CameraRotationSpeed * ts;

			if (Engine::Input::IsKeyPressed(EG_KEY_E))
				m_CameraRotation -= m_CameraRotationSpeed * ts;

			m_Camera.SetRotation(m_CameraRotation);

		}
		
		m_Camera.SetPosition(m_CameraPosition);
		m_CameraTranslationSpeed = m_ZoomLevel * 2.0f;
	}

	void OrtographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(EG_BIND_EVENT_FN(OrtographicCameraController::onMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(EG_BIND_EVENT_FN(OrtographicCameraController::onWindowResize));
	}

	bool OrtographicCameraController::onMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset() * 0.3f;	
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrtographicCameraController::onWindowResize(WindowResizeEvent& e)
	{
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();

		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

		return false;
	}
}


