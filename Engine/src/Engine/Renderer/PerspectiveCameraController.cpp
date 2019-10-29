#include "EGpch.h"
#include "PerspectiveCameraController.h"

#include "Engine/Core/Keycodes.h"
#include "Engine/Core/MouseButtonCodes.h"
#include "Engine/Core/Input.h"
#include "Engine/Core/Application.h"

namespace Engine
{
	Engine::PerspectiveCameraController::PerspectiveCameraController(float fov, float aspectRatio)
		:m_Camera(fov, aspectRatio), m_AspectRatio(aspectRatio)
	{

	}

	void PerspectiveCameraController::OnUpdate(Timestep ts)
	{
		//Camera
		if (Engine::Input::IsKeyPressed(EG_KEY_W))
			m_CameraPosition += m_Camera.GetDirection() * m_CameraMoveSpeed * ts.GetSeconds();

		if (Engine::Input::IsKeyPressed(EG_KEY_S))
			m_CameraPosition -= m_Camera.GetDirection() * m_CameraMoveSpeed * ts.GetSeconds();

		if (Engine::Input::IsKeyPressed(EG_KEY_A))
			m_CameraPosition -= m_Camera.GetVectorRight() * m_CameraMoveSpeed * ts.GetSeconds();

		if (Engine::Input::IsKeyPressed(EG_KEY_D))
			m_CameraPosition += m_Camera.GetVectorRight() * m_CameraMoveSpeed * ts.GetSeconds();

		if (Engine::Input::IsKeyPressed(EG_KEY_LEFT_SHIFT))
			m_CameraPosition -= glm::vec3(0.0f, 1.0f, 0.0f) * m_CameraMoveSpeed * ts.GetSeconds();

		if (Engine::Input::IsKeyPressed(EG_KEY_SPACE))
			m_CameraPosition += glm::vec3(0.0f, 1.0f, 0.0f) * m_CameraMoveSpeed * ts.GetSeconds();

		m_Camera.SetPosition(m_CameraPosition);

		if (Engine::Input::IsMouseButtonPressed(EG_MOUSE_BUTTON_4))
		{
			float xpos = Engine::Input::GetMouseX();
			float ypos = Engine::Input::GetMouseY();
			Engine::Application::Get().GetWindow().SetCursorPosition(1280.0f / 2.0f, 720.0f / 2.0f);
			float horizontalAngle = m_Camera.GetAngles().x;
			float verticalAngle = m_Camera.GetAngles().y;
			m_Angles.x = horizontalAngle + (m_MouseSpeed * ts * float(1280.0f / 2.0f - xpos));
			m_Angles.y = verticalAngle + (m_MouseSpeed * ts * float(720.0f / 2.0f - ypos));
			m_Camera.SetAngles(m_Angles.x, m_Angles.y);
		}
	}

	void PerspectiveCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(EG_BIND_EVENT_FN(PerspectiveCameraController::OnMouseScroll));
		dispatcher.Dispatch<MouseButtonPressedEvent>(EG_BIND_EVENT_FN(PerspectiveCameraController::OnMouseButton));
	}

	bool PerspectiveCameraController::OnMouseScroll(MouseScrolledEvent& e)
	{
		return false;
	}

	bool PerspectiveCameraController::OnMouseButton(MouseButtonPressedEvent& e)
	{
		if (e.GetMouseButton() == EG_MOUSE_BUTTON_4)
			Engine::Application::Get().GetWindow().SetCursorPosition(1280.0f / 2.0f, 720.0f / 2.0f);

		return false;
	}

}

