#include "EGpch.h"
#include "imgui.h"
#include "Engine/GUI/Gui.h"

namespace Engine
{
	const void Gui::InitDocspace()
	{
		//Docking code
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		bool p_open = NULL;

		ImGui::Begin("DockSpace Demo", &p_open, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		else
		{
			//ShowDockingDisabledMessage();
		}


		ImGui::End();
	}

	const void Gui::ViewportWindow(const Ref<FrameBuffer>& frameBuffer)
	{
		int W = frameBuffer->GetTextureSize().x;
		int H = frameBuffer->GetTextureSize().y;

		ImGui::SetNextWindowSize(ImVec2(W, H));
		//m_Framebuffer->SetTextureSize({ size.x , size.y});
		ImGuiWindowFlags flags = ImGuiTabBarFlags_NoTooltip;
		ImGui::Begin("Viewport", NULL, flags);
		{
			ImVec2 size = ImGui::GetWindowSize();
			frameBuffer->SetTextureSize({ size.x, size.y });
			ImVec2 pos = ImGui::GetCursorScreenPos();

			auto tex = frameBuffer->GetTextureID();

			ImGui::GetWindowDrawList()->AddImage(
				(void*)tex,
				ImVec2(ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y),
				ImVec2(pos.x + W, pos.y + H), ImVec2(0, 1), ImVec2(1, 0));
		}
		ImGui::End();
	}
}