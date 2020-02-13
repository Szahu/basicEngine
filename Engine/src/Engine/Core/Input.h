#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Core/Keycodes.h"
#include "Engine/Core/MouseButtonCodes.h"

#include "glm/glm.hpp"

namespace Engine
{
	class Input
	{
	public:
		inline static void Init() { return s_Instance->InitImpl(); }
		inline static bool IsKeyPressed(int keycode) { return s_Instance->IsKeyPressedImpl(keycode); }
		inline static bool IsMouseButtonPressed(int button) { return s_Instance->IsMouseButtonPressedImpl(button); }
		inline static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
		inline static float GetMouseY() { return s_Instance->GetMouseYImpl();  }
		inline static glm::vec2 GetMousePos() { return s_Instance->GetMousePosImpl(); }
		inline static bool IsGamepadButtonPressed(int keycode) { return s_Instance->IsGamepadButtonPressedImpl(keycode); }
		inline static glm::vec2 GetGamepadLeftStickPos() { return s_Instance->GetGamepadLeftStickPosImpl(); }
		inline static glm::vec2 GetGamepadRightStickPos() { return s_Instance->GetGamepadRightStickPosImpl(); }
		inline static glm::vec2 GetGamepadTriggersPos() { return s_Instance->GetGamepadTriggersImpl(); }
	protected:
		virtual bool IsKeyPressedImpl(int keycode) = 0;
		virtual bool IsMouseButtonPressedImpl(int button) = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;
		virtual glm::vec2 GetMousePosImpl() = 0;
		virtual bool IsGamepadButtonPressedImpl(int keycode) = 0;
		virtual void InitImpl() = 0;
		virtual glm::vec2 GetGamepadLeftStickPosImpl() = 0;
		virtual glm::vec2 GetGamepadRightStickPosImpl() = 0;
		virtual glm::vec2 GetGamepadTriggersImpl() = 0;
	private:
		static Input* s_Instance;
	};
}