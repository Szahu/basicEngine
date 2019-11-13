#include "EGpch.h"
#include "ViewportWindow.h"

#include "Engine/Core/Application.h"

namespace Engine
{
	ViewportWindow::ViewportWindow()
	{
		m_FrameBuffer = FrameBuffer::Create({ 1280, 720 });
		Engine::Application::Get().SetViewportWindowPointer(this);
	}

	void ViewportWindow::OnImGuiRender()
	{
		int W = m_FrameBuffer->GetTextureSize().x;
		int H = m_FrameBuffer->GetTextureSize().y;

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 4.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(W, H));
		ImGuiWindowFlags flags = ImGuiTabBarFlags_NoTooltip;
		ImGui::Begin("Viewport", NULL, flags);
		{
			ImVec2 size = ImGui::GetWindowSize();
			if (m_WindowSize.x != size.x || m_WindowSize.y != size.y)
			{
				m_WindowSize = { size.x, size.y };
				m_FrameBuffer->SetTextureSize({ m_WindowSize.x, m_WindowSize.y });
			}

			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec2 mouse_pos = ImGui::GetMousePos();
			isHovered = ImGui::IsWindowHovered();

			m_RelativeMousePosition = { mouse_pos.x - ImGui::GetWindowPos().x - 1.0f, mouse_pos.y - ImGui::GetWindowPos().y - 8.0f - ImGui::GetFontSize() };
			auto tex = m_FrameBuffer->GetTextureID();

			ImGui::GetWindowDrawList()->AddImage(
				(void*)tex,
				ImVec2(ImGui::GetItemRectMin().x - 20, ImGui::GetItemRectMin().y),
				ImVec2(pos.x + W, pos.y + H),
				ImVec2(0, 1), ImVec2(1, 0));
		}
		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
	}
}


