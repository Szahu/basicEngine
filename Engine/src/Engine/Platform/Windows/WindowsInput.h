#pragma once

#include "Engine/Core/Input.h"


namespace Engine
{
	class WindowsInput : public Input
	{
	protected:
		virtual void InitImpl() override;
		virtual bool IsKeyPressedImpl(int keycode) override;
		virtual bool IsMouseButtonPressedImpl(int button) override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
		virtual glm::vec2 GetMousePosImpl() override;
		virtual glm::vec2 GetMousePosDeltaImpl() override;
		virtual bool IsGamepadButtonPressedImpl(int keycode) override;
		virtual glm::vec2 GetGamepadLeftStickPosImpl() override;
		virtual glm::vec2 GetGamepadRightStickPosImpl() override;
		virtual glm::vec2 GetGamepadTriggersImpl() override;
	private:
		int m_GamepadID = 0;
		double prevx = 0.0;
		double prevy = 0.0;
	};
}