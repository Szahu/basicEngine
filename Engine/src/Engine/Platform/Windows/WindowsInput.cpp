#include "EGpch.h"
#include "WindowsInput.h"

#include "Engine/Core/Application.h"
#include <GLFW/glfw3.h>


namespace Engine
{
	Input* Input::s_Instance = new WindowsInput();


	void WindowsInput::InitImpl()
	{
		for (int i = 0; i < 16; i++)
		{
			if (glfwGetJoystickName(i))
			{
				m_GamepadID = i;
			}
		}
	}

	bool WindowsInput::IsKeyPressedImpl(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;

	}
	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}
	float WindowsInput::GetMouseXImpl()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		return (float)xpos; //To Deal with viewport
	}
	float WindowsInput::GetMouseYImpl()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		return (float)ypos; //To Deal with viewport
	}
	glm::vec2 WindowsInput::GetMousePosImpl()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return { (float)xpos,  (float)ypos };
	}

	bool WindowsInput::IsGamepadButtonPressedImpl(int keycode)
	{
		GLFWgamepadstate m_GamepadState;


		if (glfwGetGamepadState(m_GamepadID, &m_GamepadState))
		{
			return m_GamepadState.buttons[keycode];
		}

		return false;
	}

	glm::vec2 WindowsInput::GetGamepadLeftStickPosImpl()
	{
		GLFWgamepadstate m_GamepadState;

		if (glfwGetGamepadState(m_GamepadID, &m_GamepadState))
		{
			float x = m_GamepadState.axes[EG_GAMEPAD_AXIS_LEFT_X];
			float y = m_GamepadState.axes[EG_GAMEPAD_AXIS_LEFT_Y];

			return { x, y };
		}

		return { 0.0f, 0.0f };
	}
		
	glm::vec2 WindowsInput::GetGamepadRightStickPosImpl()
	{
		GLFWgamepadstate m_GamepadState;


		if (glfwGetGamepadState(m_GamepadID, &m_GamepadState))
		{
			float x = m_GamepadState.axes[EG_GAMEPAD_AXIS_RIGHT_X];
			float y = m_GamepadState.axes[EG_GAMEPAD_AXIS_RIGHT_Y];

			return { x, y };
		}

		return { 0.0f, 0.0f };
	}

	glm::vec2 WindowsInput::GetGamepadTriggersImpl()
	{
		GLFWgamepadstate m_GamepadState;


		if (glfwGetGamepadState(m_GamepadID, &m_GamepadState))
		{
			float x = m_GamepadState.axes[EG_GAMEPAD_AXIS_LEFT_TRIGGER];
			float y = m_GamepadState.axes[EG_GAMEPAD_AXIS_RIGHT_TRIGGER];

			return { x, y };
		}

		return { 0.0f, 0.0f };
	}
}
