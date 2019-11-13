#pragma once

#include "glm/glm.hpp"
#include "imgui.h"

#include "Engine/Renderer/FrameBuffer.h"

namespace Engine
{
	class ViewportWindow
	{
	public:
		ViewportWindow();

		void OnImGuiRender();

		const Ref<FrameBuffer>& GetFrameBuffer() { return m_FrameBuffer; }
		const glm::vec2& GetRelativeMousePos() { return m_RelativeMousePosition; }

		bool IsHovered() { return isHovered; }

	private:
		Ref<FrameBuffer> m_FrameBuffer;
		glm::vec2 m_RelativeMousePosition = { 0.0f, 0.0f };
		glm::vec2 m_WindowSize = { 0.0f, 0.0f };
		bool isHovered = false;
	};
}