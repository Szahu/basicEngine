#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Renderer/FrameBuffer.h"
#include "glm/glm.hpp"

#include "imgui.h"

namespace Engine
{
	

	class Gui
	{
	private:
		struct Style
		{
			ImVec4 ChildWindowBackground = { 35 / 255.0f, 35.0f / 255.0f, 35.0f / 255.0f, 1.0f };
			ImVec4 FrameBackground = { 61.0f / 255.0f, 61.0f / 255.0f, 61.0f / 255.0f, 1.0f };
			ImVec4 TitleBackground = { 21.0f / 255.0f, 21.0f / 255.0f, 21.0f / 255.0f, 1.0f };
		};

	public:
		static const void InitDocspace();

		static const void ViewportWindow(const Ref<FrameBuffer>& frameBuffer);

		static const const glm::vec2& GetViewportPosition() { return m_Data->m_ViewportPosition; }

		static const Style* GetStyle() { return m_Style; }
	private:

		struct ViewportData
		{
			glm::vec2 m_ViewportPosition = { 0.0f, 0.0f };
		};

		

		static Style* m_Style;

		static ViewportData* m_Data;
	};
}