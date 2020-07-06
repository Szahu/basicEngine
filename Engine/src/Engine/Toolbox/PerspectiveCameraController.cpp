#include "EGpch.h"
#include "PerspectiveCameraController.h"

#include "Engine/Core/Keycodes.h"
#include "Engine/Core/MouseButtonCodes.h"
#include "Engine/Core/Input.h"
#include "Engine/Core/Application.h"

namespace Engine
{
	Engine::PerspectiveCameraController::PerspectiveCameraController(float fov, float aspectRatio, ViewportWindow* window)
		:m_Camera(fov, aspectRatio), m_AspectRatio(aspectRatio), m_FOV(fov)
	{
		m_ActiveWindow = window;
		if (m_ActiveWindow)
		{
			m_ScreenHeight = m_ActiveWindow->GetFrameBuffer()->GetTextureSize().y;
			m_ScreenWidth = m_ActiveWindow->GetFrameBuffer()->GetTextureSize().x;
		}
		else 
		{
			m_ScreenHeight = Application::Get().GetWindow().GetHeight();
			m_ScreenWidth = Application::Get().GetWindow().GetWidth();
		}

	}

	void PerspectiveCameraController::OnUpdate(Timestep ts)
	{
		if (m_ActiveWindow)
		{
			if ((m_ScreenWidth != m_ActiveWindow->GetFrameBuffer()->GetTextureSize().x || m_ScreenHeight != m_ActiveWindow->GetFrameBuffer()->GetTextureSize().y) && m_ActiveWindow != nullptr)
			{
				OnGuiWindowResize({ m_ActiveWindow->GetFrameBuffer()->GetTextureSize().x, m_ActiveWindow->GetFrameBuffer()->GetTextureSize().y });
			}
		}
		
		float horizontalAngle = m_Camera.GetAngles().x;
		float verticalAngle = m_Camera.GetAngles().y;
		m_Rotation = glm::vec2(horizontalAngle * 180.0f / M_PI, verticalAngle * 180.0f / M_PI);

		if (m_ActiveWindow)
		{
			if (m_ActiveWindow->IsHovered())
			{
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
					Engine::Application::Get().GetWindow().SetCursorPosition(m_ScreenWidth / 2, m_ScreenHeight / 2);
					float horizontalAngle = m_Camera.GetAngles().x;
					float verticalAngle = m_Camera.GetAngles().y;
					m_Angles.x = horizontalAngle + (m_MouseSpeed * ts * float(m_ScreenWidth / 2 - xpos));
					m_Angles.y = verticalAngle + (m_MouseSpeed * ts * float(m_ScreenHeight / 2 - ypos));
					m_Camera.SetAngles(m_Angles.x, m_Angles.y);
				}

				if (Engine::Input::IsKeyPressed(EG_KEY_LEFT_ALT))
				{
					float xpos = Engine::Input::GetMouseX();
					float ypos = Engine::Input::GetMouseY();
					Engine::Application::Get().GetWindow().SetCursorPosition(m_ScreenWidth / 2, m_ScreenHeight / 2);
					float horizontalAngle = m_Camera.GetAngles().x;
					float verticalAngle = m_Camera.GetAngles().y;
					m_Angles.x = horizontalAngle + (m_MouseSpeed * ts * float(m_ScreenWidth / 2 - xpos));
					m_Angles.y = verticalAngle + (m_MouseSpeed * ts * float(m_ScreenHeight / 2 - ypos));
					m_Camera.SetAngles(m_Angles.x, m_Angles.y);
				}
			}
		}
		
		else
		{
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
				Engine::Application::Get().GetWindow().SetCursorPosition(m_ScreenWidth / 2, m_ScreenHeight / 2);
				float horizontalAngle = m_Camera.GetAngles().x;
				float verticalAngle = m_Camera.GetAngles().y;
				m_Angles.x = horizontalAngle + (m_MouseSpeed * ts * float(m_ScreenWidth / 2 - xpos));
				m_Angles.y = verticalAngle + (m_MouseSpeed * ts * float(m_ScreenHeight / 2 - ypos));
				m_Camera.SetAngles(m_Angles.x, m_Angles.y);
			}

			if (Engine::Input::IsKeyPressed(EG_KEY_LEFT_ALT))
			{
				float xpos = Engine::Input::GetMouseX();
				float ypos = Engine::Input::GetMouseY();
				Engine::Application::Get().GetWindow().SetCursorPosition(m_ScreenWidth / 2, m_ScreenHeight / 2);
				float horizontalAngle = m_Camera.GetAngles().x;
				float verticalAngle = m_Camera.GetAngles().y;
				m_Angles.x = horizontalAngle + (m_MouseSpeed * ts * float(m_ScreenWidth / 2 - xpos));
				m_Angles.y = verticalAngle + (m_MouseSpeed * ts * float(m_ScreenHeight / 2 - ypos));
				m_Camera.SetAngles(m_Angles.x, m_Angles.y);
			}
		}
		
	}

	void PerspectiveCameraController::OnEvent(Event& e)
	{
		if (m_ActiveWindow)
		{
			if (m_ActiveWindow->IsHovered())
			{
				EventDispatcher dispatcher(e);
				dispatcher.Dispatch<WindowResizeEvent>(EG_BIND_EVENT_FN(PerspectiveCameraController::OnWindowResize));
				dispatcher.Dispatch<MouseScrolledEvent>(EG_BIND_EVENT_FN(PerspectiveCameraController::OnMouseScroll));
				dispatcher.Dispatch<MouseButtonPressedEvent>(EG_BIND_EVENT_FN(PerspectiveCameraController::OnMouseButton));
			}
		}
		else
		{
			EventDispatcher dispatcher(e);
			dispatcher.Dispatch<WindowResizeEvent>(EG_BIND_EVENT_FN(PerspectiveCameraController::OnWindowResize));
			dispatcher.Dispatch<MouseScrolledEvent>(EG_BIND_EVENT_FN(PerspectiveCameraController::OnMouseScroll));
			dispatcher.Dispatch<MouseButtonPressedEvent>(EG_BIND_EVENT_FN(PerspectiveCameraController::OnMouseButton));
		}
		
	}

	bool PerspectiveCameraController::OnMouseScroll(MouseScrolledEvent& e)
	{
		m_FOV -= e.GetYOffset() * 2.0f;
		m_Camera.SetProjection(std::max((int)m_FOV, 1), m_AspectRatio);
		return false;
	}

	bool PerspectiveCameraController::OnMouseButton(MouseButtonPressedEvent& e)
	{
		if (e.GetMouseButton() == EG_MOUSE_BUTTON_4)
			Engine::Application::Get().GetWindow().SetCursorPosition(m_ScreenWidth / 2, m_ScreenHeight / 2);

		return false;
	}

	bool PerspectiveCameraController::OnWindowResize(WindowResizeEvent& e)
	{
		if (!e.GetWidth() == 0 && !e.GetHeight() == 0)
		{
			m_Camera.SetProjection(m_FOV, (float)e.GetWidth() / (float)e.GetHeight());
			m_ScreenHeight = e.GetHeight();
			m_ScreenWidth = e.GetWidth();
			m_AspectRatio = (float)m_ScreenWidth / (float)m_ScreenHeight;
			return false;
		}
		return false;
	}

	bool PerspectiveCameraController::OnKeyPressed(KeyEvent& e)
	{
		if (e.GetKeyCode() == EG_KEY_LEFT_ALT)
			Engine::Application::Get().GetWindow().SetCursorPosition(m_ScreenWidth / 2, m_ScreenHeight / 2);

		return false;
	}

	void PerspectiveCameraController::OnGuiWindowResize(const glm::vec2& size)
	{
		if (!size.x == 0 && !size.y == 0)
		{
			m_ScreenHeight = size.y;
			m_ScreenWidth = size.x;
			m_AspectRatio = (float)m_ScreenWidth / (float)m_ScreenHeight;
			m_Camera.SetProjection(m_FOV, m_AspectRatio);
		}
	}

}

