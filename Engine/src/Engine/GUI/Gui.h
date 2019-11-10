#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Renderer/FrameBuffer.h"
#include "glm/glm.hpp"

namespace Engine
{
	class Gui
	{
	public:
		static const void InitDocspace();

		static const void ViewportWindow(const Ref<FrameBuffer>& frameBuffer);

		static const const glm::vec2& GetViewportPosition() { return m_Data->m_ViewportPosition; }

	private:

		struct ViewportData
		{
			glm::vec2 m_ViewportPosition = { 0.0f, 0.0f };
		};

		static ViewportData* m_Data;
	};
}